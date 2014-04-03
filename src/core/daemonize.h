#ifndef DAEMONIZE_H
#define DAEMONIZE_H

#include "server.h"

int daemonize(int argc, char *argv[], struct SERVER *server);

#endif /* DAEMONIZE_H */