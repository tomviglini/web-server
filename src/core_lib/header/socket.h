#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <dirent.h>
#include <limits.h>
#include <time.h>
#include <netdb.h>

#include <netinet/tcp.h>
#include <arpa/inet.h>

void 	_socket_create		(int *socket_fd);
void 	_socket_bind		(int *socket_fd, int *return_value, struct sockaddr_in *address, char *ip_address, char *port_address);
void 	_socket_set_options	(int *socket_fd, int *optval);
void 	_socket_set_options2    (int *socket_fd, int *optval);
void 	_socket_set_non_bloking	(int *socket_fd, int *return_value, int *flags);
void 	_socket_listen		(int *socket_fd, int *return_value);
int 	_socket_accept		(int *socket_fd, int *accepted_fd, struct sockaddr_in *address, socklen_t *address_len);

#endif /* SOCKET_H */
