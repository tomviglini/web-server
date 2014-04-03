web-server
--------------------------------------------------

A web server written in C using the linux epoll interface and partially implemented HTTP 1.1. It can handle static files as well as C servlets. The C source files are compiled on the fly and loaded as a Shared Object and recompiled when the file changes using inotify.
