#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <unistd.h>

enum path_type {
	PATH_TYPE_STATIC,
	PATH_TYPE_DYNAMIC,
	PATH_TYPE_ERROR
};

struct DYNAMIC {

	// function pointer
	int (*function) (int, char *[]);

	// shared object handler
	void *handle;

};

struct STATIC {

	// file pointer
	void *ptr;

	// file size
	ssize_t size;

	// file checksum "for http etag"
	char crc[33];

	// http header total size
	// HTTP/1.1 200 OK
	// Content-Length: 1000
	ssize_t header1_size;

	// http header size of the first part of the header that can be changed in case of a parcial request
	// HTTP/1.1 206 Partial Content
	// Content-Range: bytes 0-99/1000
	// Content-Length: 100
	ssize_t header2_size;

};

/*struct ERROR {
	void *ptr;
	ssize_t size;
	char crc[33];

	// test
	ssize_t header_size;
};*/

struct PATH {
	int path_type;
	void *path;
};

struct RELATIVE_PATH {
	struct RECORD *record;
};

void path_free(struct PATH *path);

#endif /* FILE_H */
