//#include <thenext.h>
#include <server.h>
#include <string.h>

int action1(int argc, char *argv[]) {
	_print(_system(argv), "action1");
	return HTTP_200;
}

int action2(int argc, char *argv[]) {
	_print(_system(argv), "action2");
	return HTTP_200;
}

int action3(int argc, char *argv[]) {
	_print(_system(argv), "action3");
	return HTTP_200;
}
