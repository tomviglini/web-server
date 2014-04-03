#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>

#include "listener.h"
#include "connection.h"
#include "queue.h"
#include "worker.h"
#include "list.h"
#include "database.h"


struct SERVER {
	struct LISTENER 		listener;
	struct CONNECTION 		connection;
	struct QUEUE 			connection_queue;
	struct WORKER			worker;
	struct LIST 			config;
	struct DATABASE 		database;

	struct LIST 			mime;

	pthread_mutex_t  		big_lock;
	pthread_cond_t  		big_cond;

	char *SERVER_ROOT;
	char *SERVER_TMP;
	char *SERVER_INCLUDE;
	char *SERVER_CFG;
	char *SERVER_PID;
	unsigned int script_num;


	int 				epoll_fd;
	struct epoll_event 	*event;


};

//__attribute__((always_inline))
struct SERVER *
_server(char *argv[]) {
	return (struct SERVER *) argv[2];
}


void server_init	(struct SERVER *server);
void server_create	(struct SERVER **server);

#endif /* SERVER_H */
