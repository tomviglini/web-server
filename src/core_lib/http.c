#include <stdlib.h>

#include "header/http.h"

/* Headers To be used
Accept-Encoding
Connection
Cookie
Content-Length
Content-Type
Host
If-None-Match
If-Range
Range
*/

/*Methods Allowed
GET
HEAD
POST
*/

#define _HTTP_1_0 808333615
#define _HTTP_1_1 825110831
#define _HTTP 1347703880

#define _GET 542393671
#define _POST 1414745936
#define _HEAD 1145128264

#define _HOST_1 1953722184
#define _HOST_2 8250

#define _CONNECTION_1 1852731203
#define _CONNECTION_2 1769235301
#define _CONNECTION_3 540700271

#define _IF_NONE_MATCH_1 1311598153
#define _IF_NONE_MATCH_2 761622127
#define _IF_NONE_MATCH_3 1668571469
#define _IF_NONE_MATCH_4 14952

#define _CONTENT_1 1953394499
#define _CONTENT_2 762605157

#define _CONTENT_LENGTH_1 1735288140
#define _CONTENT_LENGTH_2 540698740

#define _CONTENT_TYPE_1 1701869908
#define _CONTENT_TYPE_2 8250

#define _APPLICATION_URL_ENCODED_1 1886413088
#define _APPLICATION_URL_ENCODED_2 1633905004
#define _APPLICATION_URL_ENCODED_3 1852795252
#define _APPLICATION_URL_ENCODED_4 1999468591
#define _APPLICATION_URL_ENCODED_5 1714255735
#define _APPLICATION_URL_ENCODED_6 762147439
#define _APPLICATION_URL_ENCODED_7 1701606005
#define _APPLICATION_URL_ENCODED_8 1685021550
#define _APPLICATION_URL_ENCODED_9 168649829

#define _COOKIE_1 1802465091
#define _COOKIE_2 540697961


#define _CRLF 2573



__attribute__((always_inline))
int
http_parse(struct HTTP_REQUEST *http, char *request, char *buffer, register int len, int *error) {

	/* ------------------------------------------------------------------------------------------ */
	register int i;
	register int n;
	int skip;

	char *str_content_length;
	/* ------------------------------------------------------------------------------------------ */

	/* ------------------------------------------------------------------------------------------ */

	if(len >= sizeof("XXX / HTTP/1.1\r\n\r\n")) {

		switch (*((unsigned int *) request)) {

			case _GET:

				http->Method = HTTP_METHOD_GET;
				i = sizeof("GET");

				break;

			case _POST:

				if(request[4] == ' ') {
					http->Method = HTTP_METHOD_POST;
					i = sizeof("POST");
				} else {
					*error = HTTP_ACTION_405;
					return 1;
				}

				break;

			case _HEAD:

				if(request[4] == ' ') {
					http->Method = HTTP_METHOD_HEAD;
					i = sizeof("HEAD");
				} else {
					*error = HTTP_ACTION_405;
					return 1;
				}

				break;

			default:

				*error = HTTP_ACTION_405;
				return 1;

				break;
		}


	} else {

		// 400 Bad request
		*error = HTTP_ACTION_400;
		return 1;
	}
	/* ------------------------------------------------------------------------------------------ */

	/* ------------------------------------------------------------------------------------------ */
	n = 0;

	skip = 0;

	http->Path = &buffer[n];

	while(request[i] != ' ' && i < len) {

		if(request[i] == '?') {
			skip = 1;
			i++;
			break;
		}

		buffer[n] = request[i];

		n++;
		i++;
	}

	buffer[n] = '\0';
	n++;
	/* ------------------------------------------------------------------------------------------ */


	/* ------------------------------------------------------------------------------------------ */
	if(skip == 1) {

		skip = 0;

		http->Query_String = &buffer[n];

		while(request[i] != ' ' && i < len) {

			buffer[n] = request[i];

			n++;
			i++;
		}

		buffer[n] = '\0';
		n++;

	} else {

		http->Query_String = NULL;

	}

	i++;
	/* ------------------------------------------------------------------------------------------ */

	/* ------------------------------------------------------------------------------------------ */

	if(len > i + sizeof("HTTP/X.X\r\n\r\n") - 1) {

		if(*((unsigned int *) &request[i]) == _HTTP) {

			switch (*((unsigned int *) &request[i + 4])) {

				case _HTTP_1_0: http->Version = HTTP_VERSION_1_0; break;
				case _HTTP_1_1: http->Version = HTTP_VERSION_1_1; break;

				default:

					// 400 Bad request
					*error = HTTP_ACTION_400;
					return 1;

					break;

			}

			i += sizeof("HTTP/X.X\r\n") - 1;

		}

	} else {

		// 400 Bad request
		*error = HTTP_ACTION_400;
		return 1;

	}

	/* ------------------------------------------------------------------------------------------ */
	http->Header.Host = NULL;
	http->Body = NULL;
	http->Header.Content_Length = 0;
	http->Header.Content_Type = HTTP_CONTENT_TYPE_NULL;
	http->Header.If_None_Match = NULL;
	/* ------------------------------------------------------------------------------------------ */


	/* ------------------------------------------------------------------------------------------ */
	/*http->CONTENT_LENGTH = 0;
	http->CONTENT_TYPE = HTTP_CONTENT_TYPE_NULL;
	http->IF_NONE_MATCH[0] = '\0';
	http->BODY[0] = '\0';*/
	/* ------------------------------------------------------------------------------------------ */




	while(1) {

		if(len >= i + sizeof(int)) {

			switch (*((unsigned int *) &request[i])) {

				case _HOST_1:

					/*write(1, "HOST: ", sizeof("HOST: ") - 1);
					if(request[i+0] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+0] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+0], 1); }
					if(request[i+1] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+1] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+1], 1); }
					if(request[i+2] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+2] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+2], 1); }
					if(request[i+3] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+3] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+3], 1); }
					write(1, "\n", sizeof("\n") - 1);
					sleep(1);*/

					if(*((unsigned short *) &request[i + 4]) == _HOST_2) {

						i += sizeof("Host: ") - 1;

						http->Header.Host = &buffer[n];

						while(request[i] != '\r' && i < len) {

							buffer[n] = request[i];

							n++;
							i++;
						}

						buffer[n] = '\0';
						n++;

						i += 2;

					} else {
						i += sizeof(int);
						skip = 1;
					}

					break;

				case _CONNECTION_1:

					/*write(1, "CONNECTION: ", sizeof("CONNECTION: ") - 1);
					if(request[i+0] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+0] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+0], 1); }
					if(request[i+1] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+1] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+1], 1); }
					if(request[i+2] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+2] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+2], 1); }
					if(request[i+3] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+3] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+3], 1); }
					write(1, "\n", sizeof("\n") - 1);
					sleep(1);*/


					if(		*((unsigned int *) &request[i + 4]) == _CONNECTION_2 &&
							*((unsigned int *) &request[i + 8]) == _CONNECTION_3) {

						i += sizeof("Connection: ") - 1;

						// LAZY CHECK TO BE REVISED CONSIDER THE HTTP/1.0/HTTP/1.1 'K-k' && 'A-a' VARIABLE
						if(request[i] == 'C') {
							http->Header.Connection = HTTP_CONNECTION_CLOSE;
							i += sizeof("Close\n\r") - 1;
						} else {
							http->Header.Connection = HTTP_CONNECTION_KEEP_ALIVE;
							i += sizeof("Keep-Alive\n\r") - 1;
						}
						// LAZY CHECK TO BE REVISED

					} else {
						i += sizeof(int);
						skip = 1;
					}

					break;

				case _IF_NONE_MATCH_1:

					/*write(1, "IF_NONE_MATCH: ", sizeof("IF_NONE_MATCH: ") - 1);
					if(request[i+0] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+0] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+0], 1); }
					if(request[i+1] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+1] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+1], 1); }
					if(request[i+2] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+2] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+2], 1); }
					if(request[i+3] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+3] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+3], 1); }
					write(1, "\n", sizeof("\n") - 1);
					sleep(1);*/

					if(		*((unsigned int *) 		&request[i + 4]) 	== _IF_NONE_MATCH_2 &&
							*((unsigned int *) 		&request[i + 8]) 	== _IF_NONE_MATCH_3 &&
							*((unsigned short *) 	&request[i + 12]) 	== _IF_NONE_MATCH_4) {

						i += sizeof("If-None-Match: ") - 1;

						// to be optimized
						http->Header.If_None_Match = &buffer[n];

						memcpy(&buffer[n], &request[i], 32);

						n += 32;

						buffer[n] = '\0';
						n++;
						// to be optimized


						i += 32 + sizeof("\r\n") - 1;

					} else {
						i += sizeof(int);
						skip = 1;
					}

					break;

				case _CONTENT_1:

					/*write(1, "CONTENT: ", sizeof("CONTENT: ") - 1);
					if(request[i+0] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+0] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+0], 1); }
					if(request[i+1] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+1] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+1], 1); }
					if(request[i+2] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+2] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+2], 1); }
					if(request[i+3] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+3] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+3], 1); }
					write(1, "\n", sizeof("\n") - 1);
					sleep(1);*/

					if(*((unsigned int *) &request[i + 4]) == _CONTENT_2) {

						switch (*((unsigned int *) &request[i + 8])) {

							case _CONTENT_LENGTH_1:

								if(*((unsigned int *) &request[i + 12]) == _CONTENT_LENGTH_2) {

									i += sizeof("Content-Length: ") - 1;

									str_content_length = &buffer[n];

									while(request[i] != '\r' && i < len) {

										buffer[n] = request[i];

										n++;
										i++;
									}

									buffer[n] = '\0';
									n++;

									http->Header.Content_Length = atoi(str_content_length);

									i += 2;

								} else {
									i += sizeof(int);
									skip = 1;
								}

								break;

							case _CONTENT_TYPE_1:

								if(*((unsigned short *) &request[i + 12]) == _CONTENT_TYPE_2) {

									i += sizeof("Content-Type: ") - 2;

									if(		*((unsigned int *) 	&request[i]) 		== _APPLICATION_URL_ENCODED_1 &&
											*((unsigned int *) 	&request[i + 4]) 	== _APPLICATION_URL_ENCODED_2 &&
											*((unsigned int *) 	&request[i + 8]) 	== _APPLICATION_URL_ENCODED_3 &&
											*((unsigned int *) 	&request[i + 12]) 	== _APPLICATION_URL_ENCODED_4 &&
											*((unsigned int *) 	&request[i + 16]) 	== _APPLICATION_URL_ENCODED_5 &&
											*((unsigned int *) 	&request[i + 20]) 	== _APPLICATION_URL_ENCODED_6 &&
											*((unsigned int *) 	&request[i + 24]) 	== _APPLICATION_URL_ENCODED_7 &&
											*((unsigned int *) 	&request[i + 28]) 	== _APPLICATION_URL_ENCODED_8 &&
											*((unsigned int *) 	&request[i + 32]) 	== _APPLICATION_URL_ENCODED_9) {

										i += sizeof(" application/x-www-form-urlencoded\r\n") - 1;

										http->Header.Content_Type = HTTP_CONTENT_TYPE_APPLICATION_URL_ENCODED;

									} else {
										i += sizeof(int);
										skip = 1;
									}

								} else {
									i += sizeof(int);
									skip = 1;
								}

								break;

							default:

								i += sizeof(int) * 2;
								skip = 1;

								break;
						}

					} else {
						i += sizeof(int);
						skip = 1;
					}

					break;

				case _COOKIE_1:

					/*write(1, "COOKIE: ", sizeof("COOKIE: ") - 1);
					if(request[i+0] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+0] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+0], 1); }
					if(request[i+1] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+1] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+1], 1); }
					if(request[i+2] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+2] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+2], 1); }
					if(request[i+3] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+3] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+3], 1); }
					write(1, "\n", sizeof("\n") - 1);
					sleep(1);*/

					if(*((unsigned int *) &request[i + 4]) == _COOKIE_2) {

						i += sizeof("Cookie: ") - 1;

						http->Header.Cookie = &buffer[n];

						while(request[i] != '\r' && i < len) {

							buffer[n] = request[i];

							n++;
							i++;
						}

						buffer[n] = '\0';
						n++;

						i += 2;


					} else {
						i += sizeof(int);
						skip = 1;
					}

					break;

				default:

					/*write(1, "NADA: ", sizeof("NADA: ") - 1);
					if(request[i+0] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+0] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+0], 1); }
					if(request[i+1] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+1] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+1], 1); }
					if(request[i+2] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+2] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+2], 1); }
					if(request[i+3] == '\r') { write(1, "<CR>", sizeof("<CR>") - 1); } else if(request[i+3] == '\n') { write(1, "<LF>", sizeof("<LF>") - 1); } else { write(1, &request[i+3], 1); }
					write(1, "\n", sizeof("\n") - 1);
					sleep(1);*/

					//printf("CRLF\n");

					if(*((unsigned short *) &request[i]) == _CRLF) {

						//printf("1\n");

						if(http->Header.Content_Length != 0) {

							//printf("2\n");

							http->Body = &buffer[n];

							//printf("3\n");

							memcpy(http->Body, &request[i + 2], http->Header.Content_Length);

							//printf("4\n");

							http->Body[http->Header.Content_Length] = '\0';

							//printf("5\n");

							n += http->Header.Content_Length;

							//printf("6\n");

							return 0;

						} else {

							return 0;

						}

					} else {

						skip = 1;

					}

					break;
			}

			if(i >= len) {
				break;
			}

			if(skip != 0) {
				while(request[i] != '\n' && i < len) {
					i++;
				}

				i++;

				skip = 0;
			}

		} else {
			break;
		}

	}
	/* ------------------------------------------------------------------------------------------ */

	/* ------------------------------------------------------------------------------------------ */
	/*if(strcmp(http->Path, "/") == 0) {
		strcpy(http->Path, "/index.htm");
	}*/
	/* ------------------------------------------------------------------------------------------ */

	return 0;

}

__attribute__((always_inline))
void
http_print_buffer(char *buffer, int size) {
	int i;

	for(i = 0; i < size; i++) {

		switch (buffer[i]) {

			case '\r':

				printf("<CR>");

				break;

			case '\n':

				printf("<LF>\n");
				printf("\n");

				break;

			default:

				printf("%c", buffer[i]);

				break;
		}

	}

	printf("\n");

}

__attribute__((always_inline))
void
http_print_env(struct HTTP_REQUEST *Request) {

	printf("METHOD: ");
	switch (Request->Method) {
		case HTTP_METHOD_GET: 	printf("GET"); break;
		case HTTP_METHOD_POST: 	printf("POST"); break;
		case HTTP_METHOD_HEAD: 	printf("HEAD"); break;
	}
	printf("\n");

	printf("VERSION: ");
	switch (Request->Version) {
		case HTTP_VERSION_1_0: printf("HTTP/1.0"); break;
		case HTTP_VERSION_1_1: printf("HTTP/1.1"); break;
	}
	printf("\n");

	printf("PATH: %s\n", Request->Path);

	if(Request->Query_String != NULL) {
		printf("QUERY_STRING: %s\n", Request->Query_String);
	}

	if(Request->Header.Host != NULL) {
		printf("HOST: %s\n", Request->Header.Host);
	}

	if(Request->Header.Connection != HTTP_CONNECTION_NULL) {
		printf("CONNECTION: ");
		switch (Request->Header.Connection) {
			case HTTP_CONNECTION_CLOSE: 		printf("CLOSE"); break;
			case HTTP_CONNECTION_KEEP_ALIVE: 	printf("KEEP-ALIVE"); break;
		}
		printf("\n");
	}

	if(Request->Header.Content_Length != 0) {
		printf("CONTENT-LENGTH: %d\n", Request->Header.Content_Length);
	}

	if(Request->Header.Content_Type != HTTP_CONTENT_TYPE_NULL) {
		printf("CONTENT-TYPE: ");
		switch (Request->Header.Content_Type) {
			case HTTP_CONTENT_TYPE_APPLICATION_URL_ENCODED: printf("APPLICATION_URL_ENCODED"); break;
		}
		printf("\n");
	}

	if(Request->Header.Content_Length != 0) {
		printf("BODY: %s\n", Request->Body);
	}

	if(Request->Header.Cookie != NULL) {
		printf("COOKIE: %s\n", Request->Header.Cookie);
	}

	if(Request->Header.If_None_Match != NULL) {
		printf("IF_NONE_MATCH: %s\n", Request->Header.If_None_Match);
	}

	printf("\n");

}

__attribute__((always_inline))
void
http_simulate		(struct HTTP_REQUEST *http_request, char *buffer) {
	http_request->Method 			= HTTP_METHOD_GET;
	http_request->Version 			= HTTP_VERSION_1_1;
	http_request->Header.Connection = HTTP_CONNECTION_KEEP_ALIVE;

	int n = 0;

	http_request->Header.Host = &buffer[n];
	strcpy(&buffer[n], "192.168.1.200");
	n += sizeof("192.168.1.200");

	http_request->Path = &buffer[n];
	strcpy(&buffer[n], "/hello.htm");
	n += sizeof("/hello.htm");

	http_request->Header.If_None_Match = &buffer[n];
	strcpy(&buffer[n], "12345678901234567890123456789012");
	n += sizeof("12345678901234567890123456789012");
}
