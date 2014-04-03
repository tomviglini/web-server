#ifndef EVENT_H
#define EVENT_H

void 	_epoll_create			(int *epoll_fd);
void 	_epoll_add_fd			(int *epoll_fd, int *socket_fd, struct epoll_event *event, int *return_value);

#endif /* EVENT_H */
