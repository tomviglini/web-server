#include <unistd.h>

#include "server.h"

#include "macros.h"

void server_init(struct SERVER *server) {

	int SERVER_ROOT_LEN;

	server->SERVER_ROOT = get_current_dir_name();
	SERVER_ROOT_LEN = strlen(server->SERVER_ROOT);

	server->SERVER_TMP = malloc(sizeof(char) * SERVER_ROOT_LEN + sizeof("/tmp"));
	strcpy(server->SERVER_TMP, server->SERVER_ROOT);
	strcat(server->SERVER_TMP, "/tmp");

	server->SERVER_INCLUDE = malloc(sizeof(char) * SERVER_ROOT_LEN + sizeof("/include"));
	strcpy(server->SERVER_INCLUDE, server->SERVER_ROOT);
	strcat(server->SERVER_INCLUDE, "/include");

	server->SERVER_CFG = malloc(sizeof(char) * SERVER_ROOT_LEN + sizeof("/cfg"));
	strcpy(server->SERVER_CFG, server->SERVER_ROOT);
	strcat(server->SERVER_CFG, "/cfg");

	server->SERVER_PID = malloc(sizeof(char) * SERVER_ROOT_LEN + sizeof("/PID"));
	strcpy(server->SERVER_PID, server->SERVER_ROOT);
	strcat(server->SERVER_PID, "/PID");

	linit(&server->mime);

	pthread_mutex_init	(&server->big_lock, 		NULL);
	pthread_cond_init	(&server->big_cond,			NULL);

	worker_init			(&server->worker, 			WORKER_THREAD);
	qinit				(&server->connection_queue, 100000);
	listener_init		(&server->listener, 		100);
	connection_init		(&server->connection, 		100000);
	database_init		(&server->database);

	//table_create(&server->database.current);
	//table_init(server->database.current);

	server->script_num = 0;
}

void server_create(struct SERVER **server) {
	*server = malloc(sizeof(struct SERVER));
}
