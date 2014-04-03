#ifndef HTTP_H
#define HTTP_H

#include "http_request.h"

enum HTTP_METHOD {
	HTTP_METHOD_GET,
	HTTP_METHOD_POST,
	HTTP_METHOD_HEAD
};

enum HTTP_VERSION {
	HTTP_VERSION_NULL,
	HTTP_VERSION_1_0,
	HTTP_VERSION_1_1
};

enum HTTP_CONNECTION {
	HTTP_CONNECTION_NULL,
	HTTP_CONNECTION_CLOSE,
	HTTP_CONNECTION_KEEP_ALIVE
};

enum HTTP_ACTION {
	HTTP_ACTION_304,
	HTTP_ACTION_400,
	HTTP_ACTION_405,
	HTTP_ACTION_404
};

enum HTTP_STATUS {
	HTTP_STATUS_200,
	HTTP_STATUS_206,
	HTTP_STATUS_304
};

enum HTTP_CONTENT_TYPE {
	HTTP_CONTENT_TYPE_NULL,
	HTTP_CONTENT_TYPE_APPLICATION_URL_ENCODED
};

int 	http_parse			(struct HTTP_REQUEST *http_request, char *request, char *buffer, register int len, int *error);
void 	http_print_buffer	(char *buffer, int size);
void 	http_print_env		(struct HTTP_REQUEST *Request);
void 	http_simulate		(struct HTTP_REQUEST *http_request, char *buffer);

#endif /* HTTP_H */
