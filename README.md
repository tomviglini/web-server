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

## Formatting notes

The repository includes `.clang-format` and `.editorconfig`.

To format all C headers/sources one time:

```bash
find . -type f \( -name "*.c" -o -name "*.h" \) -print0 | xargs -0 clang-format -i
```

`.editorconfig` also enforces important basics such as `LF` line endings and a final newline (`insert_final_newline = true`).
