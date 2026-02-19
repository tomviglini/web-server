# ROADMAP

## Original Prompt

I am sure this project once had an update mechanism: when a module changed, that module was rebuilt and reloaded live. I also remember that the data structure used to expose loaded modules had a very specific behavior: it created a new array and stayed non-blocking, so many reader threads could continue without lock contention. I do not remember every detail, but I do remember it was designed so readers did not have to block.

Do not search for this in git history because it will not be there. I copied the whole project, and I think this part broke at some point and I removed it.

What I want to communicate is that the current codebase was originally shaped to make it easy to run a command (now missing) that would watch files, rebuild a module, and hot-load updated code live. That is why I mention the current data-structure pattern with non-blocking reader behavior.

## Historical Context

At some point, this project had a **rudimentary module hot-reloading mechanism**. That mechanism was later removed or lost, but the code still contains traces that strongly suggest this past design existed.

Examples of those traces include:

- `README.md` still states modules are recompiled on changes via `inotify`.
- `src/core/main.c` still includes `sys/inotify.h`.
- `src/core/main.c` still declares watcher/compiler thread variables.
- the worker path still contains dual-epoch usage counters (`database.current` + `database.used[2]`) and comments referencing table-switching logic.

## Current Findings (Architecture Snapshot)

- The server currently loads one module via `dlopen("modules/module1.so", ...)` inside `parse()`.
- Dynamic handlers are resolved via `dlsym` and stored in `struct DYNAMIC`.
- Worker threads read the current epoch (`database.current`) and increment/decrement usage counters (`database.used[epoch]`) around request processing.
- The active watcher thread startup is currently commented out.

## Full Plan A (Complete Multi-Module Hot Reload)

### 1) Restore a Real Double-Buffer Route Table

- Keep two route tables/epochs.
- Readers (workers) only read a stable epoch snapshot and never block for reload work.
- Writers build the next epoch offline.

### 2) Build Route Tables by Declaration (Copy/Swap Strategy)

- Keep a route declaration list (`host`, `external path`, `module`, `symbol`).
- Build a fresh route table for the target epoch from declarations.
- Resolve symbols with `dlopen + dlsym` while building the new epoch.

### 3) Watch + Per-Module Build Pipeline

- Add a watcher thread (`inotify`) for module source directories (and optionally module makefiles).
- Queue rebuild requests per module (deduplicated to avoid flooding).
- Compiler thread rebuilds only changed modules.

### 4) Atomic Publish + Safe Reclamation

- Publish by flipping `database.current` to the new epoch.
- Wait until `database.used[old_epoch] == 0`.
- Only then free old epoch resources and `dlclose` old handles.

### 5) Runtime UX / Command

- Add a dedicated runtime flag (e.g. `-watch`) to enable hot reload behavior.
- Keep default runtime behavior unchanged when watch mode is disabled.

### 6) Documentation and Operational Notes

- Document watch mode behavior and constraints.
- Clarify that this is a hot-swap of function pointers/handles, not a full process restart.

## Risks and Constraints

- module build failures must not publish broken epochs.
- stale handles must only be closed when no worker references old epoch objects.
- route declaration drift (declarations vs exported symbols) must surface clear errors.

## Suggested Implementation Order (When Resumed)

1. Introduce explicit route declaration structures.
2. Implement epoch table builder.
3. Implement safe epoch publish/reclaim utilities.
4. Add watcher + compile queue + compiler thread.
5. Integrate `-watch` runtime flag.
6. Validate with concurrent requests during live module edits.

---

## Backup Preservation Record: `backup/server_15_12_2013`

> This section is a forensic capture so we do not lose critical historical implementation details even if the backup disappears.

### Scope Clarification

- User request references `15_12_2023`, but the available backup in this repository is `backup/server_15_12_2013`.
- Everything below is extracted from `backup/server_15_12_2013` in this workspace.

### Inventory (What exists in that backup)

Top-level structure:

- `orchestrator/`
  - `include/thenext.h`
  - `misc/{compile,bsearch.c,sysctl.conf}`
  - `modules/module1..3/{makefile,src/app.c}`
  - `object/module1.so,module2.so,module3.so`
- `server/`
  - `main`
  - `object/module1.so,module2.so,module3.so`
- `server_src/trunk/`
  - `main`, `makefile`, `PID`
  - `include/thenext.h`
  - `misc/{compile,bsearch.c,sysctl.conf}`
  - `src/` (core C sources)
  - `src2/daemonize.c`
  - `test/{main.c,makefile,main}`

Primary evidence files:

- `backup/server_15_12_2013/server_src/trunk/src/main.c`
- `backup/server_15_12_2013/server_src/trunk/src/server.h`
- `backup/server_15_12_2013/server_src/trunk/src/server.c`
- `backup/server_15_12_2013/server_src/trunk/src/database.h`
- `backup/server_15_12_2013/server_src/trunk/src/database.c`
- `backup/server_15_12_2013/server_src/trunk/makefile`
- `backup/server_15_12_2013/orchestrator/modules/module*/makefile`

## Recovered Historical Concepts (from code)

### 1) Runtime path model for dynamic artifacts

From `server_src/trunk/src/server.{h,c}`:

- Server state includes runtime paths:
  - `SERVER_ROOT`
  - `SERVER_TMP`
  - `SERVER_INCLUDE`
  - `SERVER_CFG`
  - `SERVER_PID`
- `script_num` is tracked in `struct SERVER` and initialized to `0`.
- Paths are built dynamically at startup using current working directory.

This is direct evidence the server was designed to build/load files during runtime.

### 2) Epoch-like non-blocking reader model

From `server_src/trunk/src/database.h`:

```c
struct DATABASE {
    //struct TABLE *current;
    //struct TABLE *old;
    int current;
    int used[2];
};
```

From `database.c` init:

```c
database->current = 0;
database->used[0] = 0;
database->used[1] = 0;
```

Worker path in `main.c` confirms this usage model:

- Capture current epoch index:
  - `current_database = server->database.current;` (line ~1066)
- Increment before processing:
  - `server->database.used[current_database]++;` (line ~1069)
- Decrement after processing/write paths:
  - `server->database.used[current_database]--;` (lines ~940, ~1015)

This is consistent with the remembered “readers do not block” intention.

### 3) Historical runtime compile + link + load pipeline

In `server_src/trunk/src/main.c`:

- `csp_set_name(...)` increments `script_num` (line ~175).
- `source_to_object(...)` compiles source to `tmp.o` (line ~190).
- `object_to_shared_object(...)` links `tmp.o` to `a%d.so` (line ~267).
- `load_shared_object(...)` resolves handler using `dlsym` (line ~302).
- `compile(...)` wrapper exists (line ~452).
- `load_object(...)` wrapper exists (line ~484).

Historical compile command pattern (verbatim style):

- C/C++/ObjC/ObjC++ cases are present.
- Commands built with `gcc -x ...` and executed with `popen(...)`.
- Uses `SERVER_TMP` and `SERVER_INCLUDE`.
- Flags include `-O3 -march=native -mtune=native -pipe -flto -fPIC -w`.

### 4) Dynamic route binding model at boot

`parse(struct SERVER *server)` (line ~504):

- Creates listener for `0.0.0.0:80`.
- Creates host record (`"uipp.co"`).
- Loads one module handle via:
  - `dlopen("/root/server/object/module1.so", RTLD_NOW | RTLD_LOCAL)`.
- Binds exported symbols:
  - `/action1 -> action1`
  - `/action2 -> action2`
  - `/action3 -> action3`

### 5) Watcher concept existed, but disabled in snapshot

`watcher(void *t)` exists (line ~622) and includes commented inotify logic:

- `inotify_init()`
- `inotify_add_watch("/root/server/www/app.c", IN_CLOSE_WRITE)`
- `inotify_add_watch("/root/server/cfg/routes", IN_CLOSE_WRITE)`

There is also a commented table-swap/reclaim loop in watcher area (~685+), including:

- checks against old table usage before free
- rebuild table
- swap current/old under lock

But runtime startup has watcher creation commented out:

- `//pthread_create(&twatcher, NULL, &watcher, server);` (line ~1603)

So the mechanism is present as code traces, but inactive in this backup snapshot.

### 6) Build/deployment model for modules (orchestrator)

From `orchestrator/modules/module*/makefile`:

- module1/module2:
  - compile `src/*.c` to PIC object(s)
  - link `.so`
  - move output to `../../object/<module>.so`
- module3 uses `g++` and similarly emits `.so`.

From `server_src/trunk/makefile`:

- core build compiles `src/*.c` to `main` and copies binary into `../../server/main`.

## Concrete Gaps / Fragilities Found in Backup 15_12_2013

These are important so we do not copy historical problems as-is:

1. **Hardcoded absolute paths**
   - many hot-load paths use `/root/server/...`.
   - not portable and inconsistent with dynamic `SERVER_ROOT` paths.

2. **Loader target mismatch in compile pipeline**
   - linker emits `a%d.so` using `script_num`,
   - but loader path in helper functions opens fixed `a0.so`.
   - indicates unfinished evolution.

3. **Watcher pipeline not active**
   - watcher thread declaration exists,
   - watcher launch is commented,
   - major watcher logic is commented.

4. **Reclaim logic left as comments / mixed models**
   - comments mention pointer model (`old/current` tables),
   - active struct stores integer epoch (`current`, `used[2]`).
   - transition not fully finalized.

5. **Build command error handling is weak**
   - `popen()` output is mostly ignored,
   - command success/failure not robustly validated before publish.

## What this backup proves (high confidence)

1. The project had a real **design direction** for hot-reload:
   - watch file changes,
   - rebuild dynamic module,
   - swap runtime-visible route/module state,
   - keep worker readers mostly non-blocking.

2. The **non-blocking reader pattern** (`current` + `used[2]`) is not accidental; it is tied to intended table/version switching.

3. The backup is a **partially disabled/intermediate state**, not a complete finished implementation.

## Comparison to current codebase (important for recovery)

Current `src/core/main.c` still has remnants:

- includes `sys/inotify.h`
- declares `twatcher`
- watcher thread creation is commented
- worker still uses `database.current` + `database.used[...]`
- dynamic resolution still uses `dlsym`
- module load in parse is now `dlopen("modules/module1.so", ...)`

What is missing vs historical traces:

- explicit runtime compile functions (`source_to_object`, `object_to_shared_object`, `compile`, `load_object`)
- live watcher-driven rebuild pipeline
- finalized safe publish/reclaim mechanics

## Deductions still required before implementation

Even with this forensic capture, these decisions must be made explicitly:

1. **Source-of-truth declarations**
   - how routes map to module + symbol in current layout.

2. **Change mapping**
   - which source file changes trigger which module rebuild.

3. **Atomic publish protocol**
   - exact lock/ordering rules to flip epoch safely.

4. **Safe reclamation and handle lifecycle**
   - when old handles can be `dlclose`d after `used[old] == 0`.

5. **Failure/rollback semantics**
   - if build fails, keep previous epoch and surface diagnostics.

6. **Runtime UX**
   - `-watch` (or equivalent) mode behavior and operational docs.

## Minimal Preservation Checklist (if backup disappears)

If the backup is ever lost, this ROADMAP should still preserve these facts:

- core historical files and structure location
- exact key function names
- epoch counter model (`current`, `used[2]`)
- existence and disabled state of watcher/inotify path
- dynamic compile/link/load pipeline shape
- known fragilities to avoid in reimplementation

This section is intended to be sufficient to reconstruct a robust implementation without relying on git history or the original backup files.

---

## Code Preservation Appendix (Backup 15_12_2013)

Objective: preserve exact code for relevant historical functions that are missing (or effectively missing) in current `src/core/main.c`, plus the places where those functions were called.

Source file for all snippets below:

- `backup/server_15_12_2013/server_src/trunk/src/main.c`

### A) `csp_set_name` (helper for runtime artifact versioning)

```c
__attribute__((always_inline))
static
void
csp_set_name(struct SERVER *server) {
    server->script_num++;
}
```

Caller (`object_to_shared_object`):

```c
sprintf(buffer, "gcc %s/tmp.o -o %s/a%d.so -w -shared 2>&1", server->SERVER_TMP, server->SERVER_TMP, server->script_num);

csp_set_name(server);
```

### B) `source_to_object` (runtime source -> object compile)

```c
__attribute__((always_inline))
static
int
source_to_object(struct SERVER *server, char *path, int language) {

	char *buffer;
    FILE *file;

    buffer = malloc(sizeof (char) * 256);

    switch (language) {
        case LANGUAGE_C: 		sprintf(buffer, "gcc -x c 				%s -o %s/tmp.o -I%s -w -O3 -march=native -mtune=native -pipe -flto -fPIC -c 2>&1 ", path, server->SERVER_TMP, server->SERVER_INCLUDE); break;
        case LANGUAGE_CPP: 		sprintf(buffer, "gcc -x c++ 			%s -o %s/tmp.o -I%s -w -O3 -march=native -mtune=native -pipe -flto -fPIC -c 2>&1 ", path, server->SERVER_TMP, server->SERVER_INCLUDE); break;
        case LANGUAGE_OBJ_C: 	sprintf(buffer, "gcc -x objective-c 	%s -o %s/tmp.o -I%s -w -O3 -march=native -mtune=native -pipe -flto -fPIC -c 2>&1 ", path, server->SERVER_TMP, server->SERVER_INCLUDE); break;
        case LANGUAGE_OBJ_CPP: 	sprintf(buffer, "gcc -x objective-c++ 	%s -o %s/tmp.o -I%s -w -O3 -march=native -mtune=native -pipe -flto -fPIC -c 2>&1 ", path, server->SERVER_TMP, server->SERVER_INCLUDE); break;
    }

    file = popen(buffer, "r");

    free(buffer);

    if (file == NULL) {
        printf("Failed to run command\n");
    }

    pclose(file);

    return 0;
}
```

Callers:

```c
void * compile(struct SERVER *server, int language, char *path) {
    void * handle;

    source_to_object(server, path, language);

    object_to_shared_object(server);

    handle = dlopen("/root/server/tmp/a0.so", RTLD_NOW | RTLD_LOCAL);

    if (handle == NULL) {
        printf("dlopen error: %s\n", dlerror());
        return 1;
    }

    return handle;
}
```

```c
void * load_object(struct SERVER *server, int language, char *path) {
    void * handle;

    source_to_object(server, path, language);

    object_to_shared_object(server);

    handle = dlopen("/root/server/tmp/a0.so", RTLD_NOW | RTLD_LOCAL);

    if (handle == NULL) {
        printf("dlopen error: %s\n", dlerror());
        return 1;
    }

    return handle;
}
```

### C) `object_to_shared_object` (runtime object -> shared object)

```c
__attribute__((always_inline))
static
int
object_to_shared_object(struct SERVER *server) {

    char *buffer;
    FILE *file;

    buffer = malloc(sizeof (char) * 256);

    sprintf(buffer, "gcc %s/tmp.o -o %s/a%d.so -w -shared 2>&1", server->SERVER_TMP, server->SERVER_TMP, server->script_num);

    csp_set_name(server);

    file = popen(buffer, "r");

    if (file == NULL) {
        printf("Failed to run command\n");
    }

    pclose(file);

    return 0;
}
```

Callers:

- `compile(...)` (same snippet above)
- `load_object(...)` (same snippet above)

### D) `compile` (wrapper for runtime compile+load)

```c
void * compile(struct SERVER *server, int language, char *path) {

	void * handle;

	source_to_object(server, path, language);

	object_to_shared_object(server);

	handle = dlopen("/root/server/tmp/a0.so", RTLD_NOW | RTLD_LOCAL);

	if (handle == NULL) {
		printf("dlopen error: %s\n", dlerror());
		return 1;
	}

	return handle;

}
```

Callers found in this backup snapshot:

- No active call-site was found in `main.c` (function exists but appears unused in the visible runtime path).

### E) `load_object` (duplicate wrapper path)

```c
void * load_object(struct SERVER *server, int language, char *path) {

	void * handle;

	source_to_object(server, path, language);

	object_to_shared_object(server);

	handle = dlopen("/root/server/tmp/a0.so", RTLD_NOW | RTLD_LOCAL);

	if (handle == NULL) {
		printf("dlopen error: %s\n", dlerror());
		return 1;
	}

	return handle;

}
```

Callers found in this backup snapshot:

- No active call-site was found in `main.c` (function exists but appears unused in the visible runtime path).

### F) `get_event` (inotify event reader helper)

```c
void get_event (int fd)
{
   ssize_t len, i = 0;
   char action[81+FILENAME_MAX] = {0};
   char buff[BUFF_SIZE] = {0};

   len = read (fd, buff, BUFF_SIZE);

   /*while (i < len) {
      struct inotify_event *pevent = (struct inotify_event *)&buff[i];
      char action[81+FILENAME_MAX] = {0};

      if (pevent->len)
         strcpy (action, pevent->name);

      printf("%d\n", pevent->wd);

      if (pevent->mask & IN_ACCESS)
         strcat(action, " was read");
      if (pevent->mask & IN_ATTRIB)
         strcat(action, " Metadata changed");
      if (pevent->mask & IN_CLOSE_WRITE)
         strcat(action, " opened for writing was closed");
      if (pevent->mask & IN_CLOSE_NOWRITE)
         strcat(action, " not opened for writing was closed");
      if (pevent->mask & IN_CREATE)
         strcat(action, " created in watched directory");
      if (pevent->mask & IN_DELETE)
         strcat(action, " deleted from watched directory");
      if (pevent->mask & IN_DELETE_SELF)
         strcat(action, "Watched file/directory was itself deleted");
      if (pevent->mask & IN_MODIFY)
         strcat(action, " was modified");
      if (pevent->mask & IN_MOVE_SELF)
         strcat(action, "Watched file/directory was itself moved");
      if (pevent->mask & IN_MOVED_FROM)
         strcat(action, " moved out of watched directory");
      if (pevent->mask & IN_MOVED_TO)
         strcat(action, " moved into watched directory");
      if (pevent->mask & IN_OPEN)
         strcat(action, " was opened");


      printf ("%s\n", action);

      i += sizeof(struct inotify_event) + pevent->len;

   }*/
}  /* get_event */
```

Caller (`watcher`):

```c
while (1) {
    get_event(fd);
    printf("modified\n");
}
```

### G) `handle_error` (watcher error helper)

```c
void handle_error (int error) {
   fprintf (stderr, "Error: %s\n", strerror(error));
}  /* handle_error */
```

Callers (inside `watcher`, commented code path):

```c
fd = inotify_init();
if (fd < 0) {
  handle_error (errno);
  return 1;
}

wd = inotify_add_watch(fd, "/root/server/www/app.c", IN_CLOSE_WRITE);
if (wd < 0) {
    handle_error (errno);
    return 1;
}

wd = inotify_add_watch(fd, "/root/server/cfg/routes", IN_CLOSE_WRITE);
if (wd < 0) {
    handle_error (errno);
    return 1;
}
```

### H) `watcher` (missing in current code as runnable component)

```c
void *watcher(void *t) {

	/*int result;
	int fd;
	int wd;

	fd = inotify_init();
	if (fd < 0) {
	  handle_error (errno);
	  return 1;
	}

	wd = inotify_add_watch(fd, "/root/server/www/app.c", IN_CLOSE_WRITE);
	if (wd < 0) {
		handle_error (errno);
		return 1;
	}

	wd = inotify_add_watch(fd, "/root/server/cfg/routes", IN_CLOSE_WRITE);
	if (wd < 0) {
		handle_error (errno);
		return 1;
	}

	while (1) {
		get_event(fd);
		printf("modified\n");
	}*/

    /*struct SERVER *server;
    struct TABLE *table;
    int flags = 0;
    server = t;

    table_create(&table);
    table_init(table);

    //_ftw(server, table);
    //_ftw2(server, table);

    pthread_mutex_lock(&server->big_lock);
    server->database.old 		= server->database.current;
    server->database.current 	= table;
    pthread_mutex_unlock(&server->big_lock);*/

    /*int i;
    for(i = 0; i < table->rpath.size; i++) {
            printf("%s\n", table->rpath.record[i]->key);
    }*/

    /*while(1) {

            sleep(4);

            if(server->database.old != NULL) {

                    if(server->database.old->used != 0) {
                            //printf("continue: %d\n", server->database.old->used);
                            continue;
                    }

                    // free the table and their records
                    table_free(server->database.old);

            }

            table_create(&table);
            table_init(table);

            _ftw(server, table);

            pthread_mutex_lock(&server->big_lock);

            server->database.old 		= server->database.current;
            server->database.current 	= table;

            pthread_mutex_unlock(&server->big_lock);

    }*/

    pthread_exit(t);
}
```

Caller in `main` (startup, currently commented in backup):

```c
/*-----------------> watcher <-----------------*/
//pthread_create(&twatcher, NULL, &watcher, server);
/*-----------------> watcher <-----------------*/
```

### Practical note for reconstruction

The preserved code above is intentionally literal (including commented historical blocks and hardcoded paths), because the purpose is forensic traceability, not direct copy/paste into current production code.
