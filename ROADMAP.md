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
