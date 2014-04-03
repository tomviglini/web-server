#include "header/socket.h"

#include "../core/macros.h"

__attribute__((always_inline))
void
_socket_create(int *socket_fd) {

	*socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(*socket_fd == -1) {
		printf("socket error\n");
		abort();
	}

}

__attribute__((always_inline))
void
_socket_bind(int *socket_fd, int *return_value, struct sockaddr_in *address, char *ip_address, char *port_address) {

	memset(address, 0, sizeof(*address));

	address->sin_family 		= AF_INET;
	address->sin_port 			= htons(atoi(port_address));
	address->sin_addr.s_addr 	= inet_addr(ip_address);

	*return_value = bind(*socket_fd, (const struct sockaddr *) address, sizeof(*address));

	if(*return_value != 0) {
		printf("bind error\n");
		abort();
	}

}

__attribute__((always_inline))
void
_socket_set_options(int *socket_fd, int *optval) {

	*optval = 1;
	setsockopt(*socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *) optval, sizeof(*optval));

}

__attribute__((always_inline))
void
_socket_set_options2(int *socket_fd, int *optval) {

	*optval =  1;
	setsockopt(*socket_fd, IPPROTO_TCP, TCP_NODELAY, (char *) optval, sizeof(*optval));

}

__attribute__((always_inline))
void
_socket_set_non_bloking(int *socket_fd, int *return_value, int *flags) {

	*flags = 1;
	*return_value = ioctl(*socket_fd, FIONBIO, flags);

	if(*return_value == -1) {
		printf("ioctl error\n");
	}

}

__attribute__((always_inline))
void
_socket_listen(int *socket_fd, int *return_value) {

	*return_value = listen(*socket_fd, BACKLOG);

	if(*return_value == -1) {
		printf("listen error\n");
		abort();
	}

}

__attribute__((always_inline))
int
_socket_accept(int *socket_fd, int *accepted_fd, struct sockaddr_in *address, socklen_t *address_len) {

	//*accepted_fd = accept(*socket_fd, (struct sockaddr *) address, address_len);

	/* the kernel 2.6.28 en adelante */

	*accepted_fd = accept4(*socket_fd, (struct sockaddr *) address, address_len, O_NONBLOCK);

	if(*accepted_fd == -1) {
		if(errno == EAGAIN || errno == EWOULDBLOCK) {
			return 1;
		} else {
			printf("acept error\n");

			/*if(errno == ECONNABORTED) {
				printf("ECONNABORTED\n");
			}*/

			return 1;
		}
	}

	return 0;
}
