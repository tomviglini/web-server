#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include "string.h"

struct HTTP_RESPONSE_HEADER {
	char *							Content_Type;
	char *							Content_Encoding;
	int 							Connection;
};
struct HTTP_RESPONSE {
	int 							Version;
	struct HTTP_RESPONSE_HEADER 	Header;

	struct String					Header2;
	struct String					Header3;
	struct String					Body;
};

#endif /* HTTP_RESPONSE_H */
