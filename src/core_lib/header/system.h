#ifndef SYSTEM_H
#define SYSTEM_H

#include <core_lib/header/http_request.h>
#include <core_lib/header/http_response.h>

struct SYSTEM {
	struct HTTP_REQUEST 	Request;
	struct HTTP_RESPONSE 	Response;
};

void system_init(struct SYSTEM *System);

#endif /* SYSTEM_H */
