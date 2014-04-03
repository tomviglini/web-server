#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>

#include "../core_lib/header/listener.h"
#include "../core_lib/header/connection.h"
#include "queue.h"
#include "../core_lib/header/worker.h"
#include "../core_lib/header/list.h"
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


void server_init	(struct SERVER *server);
void server_create	(struct SERVER **server);

#endif /* SERVER_H */
