#define _GNU_SOURCE
#define _XOPEN_SOURCE 500


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/inotify.h>
#include <sys/ioctl.h>
#include <signal.h>





#include <unistd.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <dirent.h>
#include <limits.h>
#include <time.h>
#include <netdb.h>
#include <ftw.h>

#include <netinet/tcp.h>
#include <arpa/inet.h>

#include "macros.h"

#include "queue.h"
#include <core_lib/header/list.h>
#include <core_lib/header/file.h>
#include "server.h"
#include "database.h"
#include <core_lib/header/worker.h>
#include <core_lib/header/socket.h>
#include "event.h"
#include <core_lib/header/listener.h>
#include <core_lib/header/connection.h>
#include <core_lib/header/system.h>
#include <core_lib/header/http.h>
#include <core_lib/header/string.h>

#include "daemonize.h"


enum http_status_code {
	HTTP_200,
	HTTP_206,
	HTTP_404,
	HTTP_500
};


struct HTTP_STATUS_CODE_LINE {
    int len;
    char *ptr;
};

struct HTTP_STATUS_CODE_LINE http_status[] = {
    [HTTP_200] = { .len = sizeof ("HTTP/1.1 200 OK\r\n")                    - 1, .ptr = "HTTP/1.1 200 OK\r\n"},
    [HTTP_206] = { .len = sizeof ("HTTP/1.1 206 Partial Content\r\n")       - 1, .ptr = "HTTP/1.1 206 Partial Content\r\n"},
    [HTTP_404] = { .len = sizeof ("HTTP/1/1 404 Not Found\r\n")             - 1, .ptr = "HTTP/1/1 404 Not Found\r\n"},
    [HTTP_500] = { .len = sizeof ("HTTP/1/1 500 Internal Server Error\r\n") - 1, .ptr = "HTTP/1.1 500 Internal Server Error\r\n"}
};



static const char error_505[] =
        "HTTP/1.1 505 HTTP Version Not Supported\r\n"
        "Server: TheNext\r\n"
        "\r\n";
static const char error_501[] =
        "HTTP/1.1 501 Not Implemented\r\n"
        "Server: TheNext\r\n"
        "\r\n";
static const char error_416[] =
        "HTTP/1.1 416 Requested Range Not Satisfiable\r\n"
        "Server: TheNext\r\n"
        "\r\n";
static const char error_414[] =
        "HTTP/1.1 414 Request-URI Too Long\r\n"
        "Server: TheNext\r\n"
        "\r\n";
static const char error_413[] =
        "HTTP/1.1 413 Request Entity Too Large\r\n"
        "Server: TheNext\r\n"
        "\r\n";
static const char error_405[] =
        "HTTP/1.1 405 Method Not Allowed\r\n"
        "Server: TheNext\r\n"
        "\r\n";
static const char error_404[] =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 89\r\n"
        "Connection: Keep-Alive\r\n"
        "Server: TheNext\r\n"
        "\r\n"
        "<html>"
        "<head>"
        "<title>404 Not Found</title>"
        "</head>"
        "<body>"
        "<h1>404 Not Found</h1>"
        "</body>"
        "</html>";
static const char error_403[] =
        "HTTP/1.1 403 Forbidden\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 89\r\n"
        "Connection: Keep-Alive\r\n"
        "Server: TheNext\r\n"
        "\r\n"
        "<html>"
        "<head>"
        "<title>403 Forbidden</title>"
        "</head>"
        "<body>"
        "<h1>403 Forbidden</h1>"
        "</body>"
        "</html>";

static const char error_400[] =
        "HTTP/1.1 400 Bad Request\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 93\r\n"
        "Connection: Keep-Alive\r\n"
        "Server: TheNext\r\n"
        "\r\n"
        "<html>"
        "<head>"
        "<title>400 Bad Request</title>"
        "</head>"
        "<body>"
        "<h1>400 Bad Request</h1>"
        "</body>"
        "</html>";
static const char error_304[] =
        "HTTP/1.1 304 Not Modified\r\n"
        "Server: TheNext\r\n"
        "\r\n";
static const char error_301[] =
        "HTTP/1.1 301 Moved Permanently\r\n"
        "Server: TheNext\r\n"
        "\r\n";




















__attribute__((always_inline))
static
int
load_shared_object(void *handle, char *function_name, struct PATH *record) {

    int (*function) (int, char *[]);

    function = (int (*)(int, char *[])) (intptr_t) dlsym(handle, function_name);

    if (function == NULL) {
        printf("dlsym error: %s\n", dlerror());
        return 1;
    }

    struct DYNAMIC *dynamic;
    dynamic = malloc(sizeof (struct DYNAMIC));

    dynamic->function = function;
    dynamic->handle = handle;

    record->path_type = PATH_TYPE_DYNAMIC;
    record->path = dynamic;

    return 0;

}



void load(struct SERVER *server, struct RECORD *record, void *handle, char *external, char *internal) {

	struct RECORD * record2;
	struct PATH * path;

	path = malloc(sizeof(struct PATH));
	load_shared_object(handle, internal, path);
	lset(record->val, &record2, external);
	record2->val = path;

}










int listen_create(struct SERVER *server, char *ip, char *port) {

	int socket_fd;
	int optval;
	int return_value;
	struct sockaddr_in address;
	int flags;



	//-----------------> listen socket <-----------------


	//
	//_socket_create(&socket_fd);
	socket_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socket_fd == -1) {
		printf("socket error\n");
		abort();
	}
	//


	//
	//_socket_set_options(&socket_fd, &optval);
	optval = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	//


	//
	//_socket_bind(&socket_fd, &return_value, &address, ip, port);
	memset(&address, 0, sizeof(address));

	address.sin_family 			= AF_INET;
	address.sin_port 			= htons(atoi(port));
	address.sin_addr.s_addr 	= inet_addr(ip);

	return_value = bind(socket_fd, (const struct sockaddr *) &address, sizeof(address));

	if(return_value != 0) {
		printf("bind error\n");
		abort();
	}
	//

	//
	//_socket_set_non_bloking(&socket_fd, &return_value, &flags);

	flags = 1;
	return_value = ioctl(socket_fd, FIONBIO, &flags);

	if(return_value == -1) {
		printf("ioctl error\n");
	}
	//

	//
	//_socket_listen(&socket_fd, &return_value);
	return_value = listen(socket_fd, BACKLOG);

	if(return_value == -1) {
		printf("listen error\n");
		abort();
	}
	//

	//
	//_epoll_add_fd(&server->epoll_fd, &socket_fd, server->event, &return_value);
	server->event->data.fd = socket_fd;
	server->event->events = EPOLLIN | EPOLLET | EPOLLRDHUP;

	return_value = epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, socket_fd, server->event);

	if(return_value == -1) {
		printf("epoll_ctl error2\n");
		abort();
	}
	//

	// set the flag active on to the accept socket
	server->listener.record[socket_fd].active = 1;

	// initialize the socket hosts list
	linit(&server->listener.record[socket_fd].host);
	//-----------------> listen socket <-----------------



	return socket_fd;

}

struct RECORD * hostname_create(struct SERVER *server, int socket_fd, char *hostname) {

	struct RECORD * record;

	// add the host to the socket list
	lset(&server->listener.record[socket_fd].host, &record, hostname);

	// allocate the hosts list
	record->val = malloc(sizeof (struct LIST));

	// initialize the host list
	linit(record->val);

	return record;

}





void parse(struct SERVER *server) {

	//listen_create_old2(server, "0.0.0.0", "80", "uipp.co");

	int socket_fd;
	struct RECORD * host;
	void *handle;




	socket_fd = listen_create(server, "0.0.0.0", "8080");
	host = hostname_create(server, socket_fd, "viglini.com:8080");

        
        
	handle = dlopen("modules/module1.so", RTLD_NOW | RTLD_LOCAL);

	if (handle == NULL) {
		printf("dlopen error: %s\n", dlerror());
		return 1;
	}


        load(server, host, handle, "/listSockets", "listSockets");
        
        

	load(server, host, handle, "/action1", "action1");
	load(server, host, handle, "/action2", "action2");
	load(server, host, handle, "/action3", "action3");




}













// this thread close the connections in the timeout

void *timer(void *t) {

    struct SERVER *server;
    unsigned int _time;
    int i;

    server = t;

    while (1) {

        _time = time(NULL);

        pthread_mutex_lock(&server->big_lock);

        for (i = 0; i < server->connection.size; i++) {

            if (server->connection.record[i].active == 1 &&
                    server->connection.record[i].busy == 0 &&
                    server->connection.record[i].pending == 0 &&
                    server->connection.record[i].timeout <= _time) {

                server->connection.record[i].active = 0;

                close(i);

            }

        }

        pthread_mutex_unlock(&server->big_lock);

        sleep(1);

    }

    pthread_exit(t);
}
























enum WORKER_STATUS {
    WORKER_LOCK = 1,
    WORKER_BUSY = 2,
    WORKER_PENDING = 4
};

struct WORKER_PARAMETER {
    struct SERVER *server;
    int worker_id;
};















void *worker(void *t) {

    char read_buffer[WORKER_THREAD_BUFFER_SIZE];
    ssize_t len;

    char read_buffer2[WORKER_THREAD_BUFFER_SIZE];

    struct SYSTEM System;
    system_init(&System);




    struct WORKER_PARAMETER *worker_parameter;
    struct SERVER *server;
    struct TABLE *table;
    struct RECORD *record1;
    struct RECORD *record2;

    char *argv[2];

    struct iovec iov[3];
    int iov_num;

    int flag;

    int current_database;


    int worker_id;
    int queue_value;
    int queue_return;

    int lock;
    int busy;
    int pending;




    int error;



    /* */
    char relative_path[256];
    int parse_result;
    int func_result;
    /* */







    worker_parameter = t;
    worker_id = worker_parameter->worker_id;
    server = worker_parameter->server;


    argv[1] = (char *) &System;

    lock = 0;
    busy = 0;
    pending = 0;


    error = 0;

    table = NULL;

    flag = 0;



    current_database = 2;








    while (1) {

        /*if(flag &~ WORKER_LOCK) {

                // turn on the flag LOCK
                flag |= WORKER_LOCK;

                // lock
                pthread_mutex_lock(&server->big_lock);
        }*/

        if (lock == 0) {

            // turn on the flag LOCK
            lock = 1;

            // lock
            pthread_mutex_lock(&server->big_lock);

        }


        if (busy == 1) {

            /* --------------------> check <-------------------- */
            if (server->worker.record[worker_id].close_while_busy == 1) {

                server->worker.record[worker_id].close_while_busy = 0;

                busy = 0;

                pending = 0;

                /* a ser remplazado start */

                /* decrement the previous used count */
                //if(table != NULL) { table->used--; }

                /* a ser remplazado end */


                /* test start */



                /* decrement the previous used count */
                if (current_database != 2) {
                    server->database.used[current_database]--;
                }

                /* test end */

                continue;

            }
            /* --------------------> check <-------------------- */

            /* --------------------> write <-------------------- */
            if (pending == 1) {

                if (iov_num == 1) {

                    len = write(queue_value, iov[0].iov_base, iov[0].iov_len);

                    /*if(len != iov[0].iov_len) {

                            printf("%d\n", len);

                            _epoll_mod_fd(server->epoll_fd, queue_value);

                    }*/

                } else {


                	/*char asd[1024];
                	sprintf(asd,
						""
                	);

                	write(
						queue_value,
						"Content-Length: 7\r\n"
						"Server: TheNext\r\n"
						"\r\n",
						strlen(
							"Content-Length: %u\r\n"
							"Server: TheNext\r\n"
							"\r\n"
						)
					);*/


                    len = writev(queue_value, iov, iov_num);

                    //writev(1, iov, iov_num);

                }



                // --------------------------------------------------------

                // *pending implementation*
                // if no all the data were written the table/connection must not be release/close

                /* a ser remplazado start */

                /* decrement the previous used count */
                //if(table != NULL) { table->used--; }

                /* a ser remplazado end */


                /* test start */

                //printf("%d\n", current_database);



                /* decrement the previous used count */
                if (current_database != 2) {
                    server->database.used[current_database]--;
                }

                //printf("2\n");

                /* test end */




                /* if the connection is close or the http version is not 1.1 and the connection is not set close */
                if (System.Request.Header.Connection == HTTP_CONNECTION_CLOSE || (System.Request.Version != HTTP_VERSION_1_1 && System.Request.Header.Connection == HTTP_CONNECTION_NULL)) {
                    close(queue_value);
                }


                // --------------------------------------------------------



                /* turn off the flag PENDING_WRITE */
                pending = 0;

            }
            /* --------------------> write <-------------------- */

            /* --------------------> read <-------------------- */
            if (server->connection.record[queue_value].pending == 1) {

                /* read */
                len = read(queue_value, read_buffer, WORKER_THREAD_BUFFER_SIZE);

                /* turn off the flag PENDING_READ */
                server->connection.record[queue_value].pending = 0;



                /* a ser remplazado start */

                /* increment the used count */
                //server->database.current->used++;

                /* get the current table */
                //table = server->database.current;

                /* a ser remplazado end */


                /* test start */

                /* en vez de sacar el puntero a la tabla global saco el numero de la tabla local, todas las tablas van a tener que updatearse para poner pasar a current 0/1*/
                current_database = server->database.current;

                /* increment the used count */
                server->database.used[current_database]++;

                /* test end */





            } else {

                /* turn off the socket flag BUSY */
                server->connection.record[queue_value].busy = 0;

                /* update the timeout */
                server->connection.record[queue_value].timeout = time(NULL) + CONNECTION_KEEP_ALIVE_TIMEOUT;

                /* turn off the worker flag BUSY */
                busy = 0;

                continue;

            }
            /* --------------------> read <-------------------- */

        } else {

            /* --------------------> queue <-------------------- */
            while (1) {

                /* pop the next socket on the pending task */
                queue_return = qpop(&server->connection_queue, &queue_value);

                if (queue_return == 1) {

                    /* wait */
                    pthread_cond_wait(&server->big_cond, &server->big_lock);

                } else {

                    /* set the using worker to the socket record */
                    server->connection.record[queue_value].worker = worker_id;

                    /* turn on flag BUSY */
                    server->connection.record[queue_value].busy = 1;

                    /* turn on the flag BUSY */
                    busy = 1;

                    break;

                }

            }

            continue;
            /* --------------------> queue <-------------------- */

        }


        /*
        // unlock
        pthread_mutex_unlock(&server->big_lock);

        // turn off the flag LOCK
        flag &=~ WORKER_LOCK;
         */


        // unlock
        pthread_mutex_unlock(&server->big_lock);

        // turn off the flag LOCK
        lock = 0;























        /* for testing printf */
        read_buffer[len] = '\0';

        /* set the pending FLAG */
        pending = 1;

        /* init iov_num */
        iov_num = 0;

        /* init buffers */
        System.Response.Header.Content_Type = NULL;


        //printf("%s\n", read_buffer);


        //printf("%d\n", len);

        //write(1, read_buffer, len);





        /* parse http */
        parse_result = http_parse(&System.Request, read_buffer, read_buffer2, len, &error);
        //http_print_env	(&System.Request);





        if (parse_result != 0) {

            switch (error) {

                case HTTP_ACTION_304:

                    iov_num = 1;
                    iov[0].iov_base = error_304;
                    iov[0].iov_len = sizeof (error_304) - 1;

                    break;

                case HTTP_ACTION_400:

                    iov_num = 1;
                    iov[0].iov_base = error_400;
                    iov[0].iov_len = sizeof (error_400) - 1;

                    break;

                case HTTP_ACTION_405:

                    iov_num = 1;
                    iov[0].iov_base = error_405;
                    iov[0].iov_len = sizeof (error_405) - 1;

                    break;

                case HTTP_ACTION_404:

                    iov_num = 1;
                    iov[0].iov_base = error_404;
                    iov[0].iov_len = sizeof (error_404) - 1;

                    break;
            }


            continue;
        }


        if (System.Request.Header.Host == NULL) {

            iov_num = 1;
            iov[0].iov_base = error_400;
            iov[0].iov_len = sizeof (error_400) - 1;

            continue;
        }











        /* search the host */
        lget(&server->listener.record[server->connection.record[queue_value].listener].host, &record1, System.Request.Header.Host);

        if (record1 == NULL) {

            /* search for the default host 0.0.0.0 */
            lget(&server->listener.record[server->connection.record[queue_value].listener].host, &record1, "0.0.0.0");

            if (record1 == NULL) {

                iov_num = 1;
                iov[0].iov_base = error_400;
                iov[0].iov_len = sizeof (error_400) - 1;

                continue;
            }

        }

        /* search the path */
        lget(record1->val, &record2, System.Request.Path);








        if (record2 != NULL) {

            struct PATH *path = record2->val;

            struct STATIC *Static;
            struct DYNAMIC *Dynamic;
            struct ERROR *Error;

            //switch (((struct PATH *)((struct RECORD *)record2->val)->val)->path_type) {

            switch (path->path_type) {

                case PATH_TYPE_DYNAMIC:


                    Dynamic = path->path;

                    // clean the buffer
                    string_truncate(&System.Response.Header2);
                    string_truncate(&System.Response.Header3);
                    string_truncate(&System.Response.Body);

                    argv[0] = ((struct RECORD *) record2->val)->key;

                    // si no defino cada vez el puntero al sistema crashea "no tendria que pasar" hay que investigar
                    argv[1] = (char *) &System;
                    
                    // para probar voy a pasarle tambien SERVER para que el script pueda hacer todo
                    argv[2] = (char *) server;

                    //func_result = ((struct DYNAMIC *)((struct PATH *)((struct RECORD *)record2->val)->val)->path)->function(1, argv);

                    func_result = Dynamic->function(2, argv);




                    iov[0].iov_base = http_status[func_result].ptr;
                    iov[0].iov_len = http_status[func_result].len;


                    //iov[0].iov_base = http_status[http_200].ptr;
					//iov[0].iov_len = http_status[http_200].len;



                    //write(1, http_status[func_result].ptr, sizeof(http_status[func_result].ptr));

                    if (System.Response.Header.Content_Type == NULL) {
                        string_printf(
							&System.Response.Header3,
							"Content-Type: text/html\r\n"
							);
                    }

                    string_printf(
                            &System.Response.Header3,
                            "Content-Length: %u\r\n"
                            "Connection: Keep-Alive\r\n"
                            "Server: TheNext\r\n"
                            "\r\n",
                            System.Response.Body.len
                            );

                    iov[1].iov_base = System.Response.Header2.ptr;
                    iov[1].iov_len = System.Response.Header2.len;

                    iov[2].iov_base = System.Response.Header3.ptr;
                    iov[2].iov_len = System.Response.Header3.len;

                    if (System.Request.Method == HTTP_METHOD_HEAD) {
                        iov_num = 3;
                    } else {
                        iov_num = 4;
                        iov[3].iov_base = System.Response.Body.ptr;
                        iov[3].iov_len = System.Response.Body.len;
                    }

                    break;

                case PATH_TYPE_STATIC:

                    Static = path->path;

                    /*if(
                            System.Request.Header.If_None_Match != NULL &&
                            ((int *)((struct STATIC *)((struct PATH *)((struct RECORD *)record2->val)->val)->path)->crc)[0] == ((int *) System.Request.Header.If_None_Match)[0] &&
                            ((int *)((struct STATIC *)((struct PATH *)((struct RECORD *)record2->val)->val)->path)->crc)[1] == ((int *) System.Request.Header.If_None_Match)[1] &&
                            ((int *)((struct STATIC *)((struct PATH *)((struct RECORD *)record2->val)->val)->path)->crc)[2] == ((int *) System.Request.Header.If_None_Match)[2] &&
                            ((int *)((struct STATIC *)((struct PATH *)((struct RECORD *)record2->val)->val)->path)->crc)[3] == ((int *) System.Request.Header.If_None_Match)[3]
                    ) {

                            iov_num = 1;
                            iov[0].iov_base = error_304;
                            iov[0].iov_len 	= sizeof(error_304) - 1;

                    } else {*/

                    if (System.Request.Method == HTTP_METHOD_HEAD) {

                        /*iov_num = 1;
                        iov[0].iov_base = ((struct STATIC *)((struct PATH *)((struct RECORD *)record2->val)->val)->path)->ptr;
                        iov[0].iov_len 	= ((struct STATIC *)((struct PATH *)((struct RECORD *)record2->val)->val)->path)->header_size;*/

                        //iov_num = 1;
                        //iov[0].iov_base = Static->ptr;
                        //iov[0].iov_len 	= Static->header_size;
                        //iov[0].iov_len 	= Static->header_size - 1;

                    } else {

                        /*iov_num = 1;
                        iov[0].iov_base = ((struct STATIC *)((struct PATH *)((struct RECORD *)record2->val)->val)->path)->ptr;
                        iov[0].iov_len 	= ((struct STATIC *)((struct PATH *)((struct RECORD *)record2->val)->val)->path)->size;*/

                        iov_num = 1;
                        iov[0].iov_base = Static->ptr;
                        iov[0].iov_len = Static->header1_size + Static->header2_size + Static->size;

                        //printf("%s\n", Static->ptr);

                        //write(1, "\n------------------------------------------------------------\n", sizeof("\n------------------------------------------------------------\n"));
                        //write(1, iov[0].iov_base, iov[0].iov_len);

                    }

                    //}


                    break;

                case PATH_TYPE_ERROR:

                    if (System.Request.Method == HTTP_METHOD_HEAD) {

                        /*iov_num = 1;

                        iov[0].iov_base = ((struct ERROR *)((struct PATH *)((struct RECORD *)record2->val)->val)->path)->ptr;
                        iov[0].iov_len 	= ((struct ERROR *)((struct PATH *)((struct RECORD *)record2->val)->val)->path)->header_size;*/

                    } else {

                        /*iov_num = 1;

                        iov[0].iov_base = ((struct ERROR *)((struct PATH *)((struct RECORD *)record2->val)->val)->path)->ptr;
                        iov[0].iov_len 	= ((struct ERROR *)((struct PATH *)((struct RECORD *)record2->val)->val)->path)->size;*/

                    }

                    break;
            }

        } else {

            iov_num = 1;
            iov[0].iov_base = error_404;
            iov[0].iov_len = sizeof (error_404) - 1;

        }


    }

    pthread_exit(t);
}




































int main(int argc, char *argv[])
{
    struct SERVER *server;
    server_create(&server);
    server_init(server);

    

    daemonize(argc, argv, server);










    /*-----------------> cpu afinity variables <-----------------*/
    int affinity_result;
    cpu_set_t cpuset;
    pthread_t thread_id;
    /*-----------------> cpu afinity variables <-----------------*/







    /*-----------------> threads variables <-----------------*/
    struct WORKER_PARAMETER worker_parameter [WORKER_THREAD];
    pthread_t tworker [WORKER_THREAD];
    pthread_t ttimer;
    pthread_t twatcher;
    pthread_t tcompiler;
    /*-----------------> threads variables <-----------------*/









    /*-----------------> variable definition <-----------------*/
    int socket_fd;
    int epoll_fd;
    int accepted_fd;
    int epoll_fd_ready;

    int i;
    int return_value;
    int optval;
    int flags;

    struct sockaddr_in address;
    socklen_t address_len;

    struct epoll_event event;
    struct epoll_event *events;

    int wakeup_counter;
    unsigned int tiempo;
    unsigned int time_counter;
    /*-----------------> variable definition <-----------------*/






    /*-----------------> cpu afinity <-----------------*/
    thread_id = pthread_self();

    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);

    affinity_result = pthread_setaffinity_np(thread_id, sizeof (cpu_set_t), &cpuset);

    if (affinity_result != 0) {
        printf("pthread_setaffinity_np error\n");
    }
    /*-----------------> cpu afinity <-----------------*/









    /*-----------------> structures initialization <-----------------*/
    address_len = sizeof (address);

    _epoll_create(&epoll_fd);

    /* test */
    server->epoll_fd 	= epoll_fd;
    server->event 		= &event;
    /* test */

    events = malloc(sizeof (event) * MAXEVENTS);

    tiempo = 0;
    /*-----------------> structures initialization <-----------------*/









    /*-----------------> parse <-----------------*/
    //parse(server, &event, &epoll_fd);
    parse(server);
    /*-----------------> parse <-----------------*/

    //return 0;

    /*-----------------> watcher <-----------------*/
    //pthread_create(&twatcher, NULL, &watcher, server);
    /*-----------------> watcher <-----------------*/

    /*-----------------> timer <-----------------*/
    pthread_create(&ttimer, NULL, timer, server);
    /*-----------------> timer <-----------------*/

    /*-----------------> workers <-----------------*/
    for (i = 0; i < WORKER_THREAD; i++) {
        worker_parameter[i].worker_id = i;
        worker_parameter[i].server = server;
        pthread_create(&tworker[i], NULL, worker, (void *) &worker_parameter[i]);
    }
    /*-----------------> workers <-----------------*/














    /*-----------------> main loop <-----------------*/
    while (1) {

        /*-----------------> initialize <-----------------*/
        wakeup_counter = 0;
        time_counter = 0;
        /*-----------------> initialize <-----------------*/

        /* wait for epoll events */
        epoll_fd_ready = epoll_wait(epoll_fd, events, MAXEVENTS, -1);

        /* lock */
        pthread_mutex_lock(&server->big_lock);

        /*-----------------> epoll loop <-----------------*/
        for (i = 0; i < epoll_fd_ready; i++) {

            if (events[i].events & EPOLLERR || events[i].events & EPOLLHUP || events[i].events & EPOLLRDHUP) {

                server->connection.record[events[i].data.fd].active = 0;

                if (server->connection.record[events[i].data.fd].busy == 1) {

                    server->connection.record[events[i].data.fd].busy = 0;
                    server->connection.record[events[i].data.fd].pending = 0;

                    server->worker.record[server->connection.record[events[i].data.fd].worker].close_while_busy = 1;

                }

                close(events[i].data.fd);

                continue;

            } else if (server->listener.size > events[i].data.fd && server->listener.record[events[i].data.fd].active == 1) {

                while (1) {

                    return_value = _socket_accept(&events[i].data.fd, &accepted_fd, &address, &address_len);
                    if (return_value == 1) {
                        break;
                    }
                    //_socket_set_non_bloking(&accepted_fd, &return_value, &flags);
                    _socket_set_options2(&accepted_fd, &optval);
                    _epoll_add_fd(&epoll_fd, &accepted_fd, &event, &return_value);

                    server->connection.record[accepted_fd].active = 1;
                    server->connection.record[accepted_fd].listener = events[i].data.fd;

                    if (time_counter == 0) {
                        time_counter = 1;
                        tiempo = time(NULL) + CONNECTION_KEEP_ALIVE_TIMEOUT;
                    }

                    server->connection.record[accepted_fd].timeout = tiempo;

                }

                continue;

            } else {

                if (events[i].events & EPOLLOUT) {

                    printf("EPOLLOUT: %d\n", events[i].data.fd);

                }

                if (events[i].events & EPOLLIN) {

                    server->connection.record[events[i].data.fd].pending = 1;

                    if (server->connection.record[events[i].data.fd].busy == 0) {

                        qpush(&server->connection_queue, events[i].data.fd);

                        wakeup_counter++;

                    }

                }

            }

        }
        /*-----------------> epoll loop <-----------------*/

        /*-----------------> worker signaling <-----------------*/
        if (wakeup_counter == 1) {

            pthread_cond_signal(&server->big_cond);

        } else if (wakeup_counter > 1) {

            pthread_cond_broadcast(&server->big_cond);

        }
        /*-----------------> worker signaling <-----------------*/


        /* unlock */
        pthread_mutex_unlock(&server->big_lock);


    }
    /*-----------------> main loop <-----------------*/




    return 0;
}
