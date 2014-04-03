#include <sys/epoll.h>

#include "macros.h"

__attribute__((always_inline))
void
_epoll_create(int *epoll_fd) {

	*epoll_fd = epoll_create(MAXEVENTS);

	if(*epoll_fd == -1) {
		printf("epoll_create error\n");
		abort();
	}

}

__attribute__((always_inline))
void
_epoll_add_fd(int *epoll_fd, int *socket_fd, struct epoll_event *event, int *return_value) {

	event->data.fd = *socket_fd;
	event->events = EPOLLIN | EPOLLET | EPOLLRDHUP;

	*return_value = epoll_ctl(*epoll_fd, EPOLL_CTL_ADD, *socket_fd, event);

	if(*return_value == -1) {
		printf("epoll_ctl error2\n");
		abort();
	}

}


void
_epoll_mod_fd(int epoll_fd, int socket_fd) {

	struct epoll_event event;

	event.data.fd = socket_fd;
	event.events = EPOLLOUT;

	epoll_ctl(epoll_fd, EPOLL_CTL_MOD, socket_fd, &event);

}
