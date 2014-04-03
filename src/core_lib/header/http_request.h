#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

struct HTTP_REQUEST_HEADER {
	int 							Connection;
	int								Content_Type;
	int								Content_Length;
	char *							Host;
	char *							Cookie;
	char *							If_None_Match;
};
struct HTTP_REQUEST {
	int 							Method;
	char *							Path;
	char *							Query_String;
	int 							Version;

	struct HTTP_REQUEST_HEADER 		Header;
	char *							Body;
};

#endif /* HTTP_REQUEST_H */
