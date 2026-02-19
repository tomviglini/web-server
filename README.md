## TheNext web-server

A web server written in C using the Linux `epoll` interface and a partial HTTP/1.1 implementation. It can handle static files as well as C servlets. The C source files are compiled on the fly, loaded as Shared Objects, and recompiled when files change using `inotify`.

This project is mainly for **hobby/learning purposes**, especially to experiment with **C, Linux internals, and networking** concepts.

## Getting started (Linux)

### 1) Install dependencies

Required to build/run:

```bash
sudo apt update
sudo apt install -y build-essential
```

Optional (recommended) for formatting C code:

```bash
sudo apt install -y clang-format
```

### 2) Build the project

From the project root:

```bash
chmod +x install.sh
./install.sh
```

This script builds:

- the core binary (`main`)
- example modules in `modules/`

### Optional: generate `compile_commands.json` (recommended for Ctrl+Click)

If your editor is not navigating correctly to C function definitions, generate a
`compile_commands.json` file from the current Make-based build.

This does **not** replace your Makefiles and does **not** change how the project compiles.

1. Install Bear:

```bash
sudo apt install -y bear
```

2. Generate the compilation database using your existing build flow:

```bash
chmod +x install.sh
bear -- ./install.sh
```

If you get a permission error, you can also run:

```bash
bear -- bash install.sh
```

This produces `compile_commands.json` at the project root, which is used by many tools
(VS Code C/C++, clangd, CLion, etc.) to resolve includes/defines and improve
Go to Definition / Ctrl+Click.

### 3) Add host entry for the example

To make the `example.com` host used by this project resolve locally, add this line to `/etc/hosts`:

```text
127.0.0.1 example.com
```

### 4) Run the server

Foreground mode:

```bash
./main
```

Daemon mode:

```bash
./main -start
```

Stop daemon:

```bash
./main -stop
```

Then open the example host using port `8080`:

```text
http://example.com:8080
```

## Quick guide: key places to edit (simple example)

If you want to modify the example quickly, these are the most important places.

### 1) Where routes and modules are loaded

file `src/core/main.c`

```c
void parse(struct SERVER *server) {
    int socket_fd;
    struct RECORD *host;
    void *handle;

    socket_fd = listen_create(server, "0.0.0.0", "8080");
    host = hostname_create(server, socket_fd, "example.com:8080");

    handle = dlopen("modules/module1.so", RTLD_NOW | RTLD_LOCAL);

    load(server, host, handle, "/action1", "action1");
    load(server, host, handle, "/action2", "action2");
    load(server, host, handle, "/action3", "action3");
}
```

This is the routing map: URL path (`/action1`) -> C function name (`action1`) inside the loaded module.

### 2) Where module logic lives (`module2`)

file `src/manager/modules/module2/src/app.c`

```c
int action1(int argc, char *argv[]) {
    _print(_system(argv), "action1");
    return HTTP_200;
}

int action2(int argc, char *argv[]) {
    _print(_system(argv), "action2");
    return HTTP_200;
}
```

This is where you change behavior for each endpoint.

### 3) How to run `module2`

1. Build module2 so `modules/module2.so` exists.
2. In `src/core/main.c` inside `parse(server)`, change:
   - `dlopen("modules/module1.so", ...)`
   - to `dlopen("modules/module2.so", ...)`
3. Restart the server.

After that, `/action1`, `/action2`, `/action3` will run the functions from `module2`.

> Mental model: `parse()` decides **which URL calls which function**, and each module `app.c` decides **what that function does**.

## Formatting notes

The repository includes `.clang-format` and `.editorconfig`.

To format all C headers/sources one time:

```bash
find . -type f \( -name "*.c" -o -name "*.h" \) -print0 | xargs -0 clang-format -i
```

`.editorconfig` also enforces important basics such as `LF` line endings and a final newline (`insert_final_newline = true`).
