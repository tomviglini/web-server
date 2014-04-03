#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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

enum HTTP_CONTENT_TYPE {
	HTTP_CONTENT_TYPE_NULL,
	HTTP_CONTENT_TYPE_APPLICATION_URL_ENCODED
};

enum HTTP_STATUS {
	HTTP_STATUS_200,
	HTTP_STATUS_206,
	HTTP_STATUS_304
};

enum HTTP_HEADER {
	HTTP_HEADER_CONTENT_TYPE
};


enum http_status_code {
	HTTP_200,
	HTTP_206,
	HTTP_404,
	HTTP_500
};

enum {
	HTTP_COOKIE_EXPIRE_FIRST = 0,
	HTTP_COOKIE_EXPIRE_LAST = 4294967295
};


struct String {
	char *ptr;
	int allocated;
	int len;
};

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


struct SYSTEM {
	struct HTTP_REQUEST 			Request;
	struct HTTP_RESPONSE 			Response;
};





//__attribute__((always_inline))
struct SYSTEM *
_system(char *argv[]) {
	return (struct SYSTEM *) argv[1];
}


typedef struct _cookie_options {
	char *path;
	char *domain;
	char *value;
	unsigned int expire;
	unsigned int secure;
	unsigned int httponly;
} CookieOpt;



//__attribute__((always_inline))
void
_print(struct SYSTEM *System, const char *source) {

	register int i = 0;

	do {
		System->Response.Body.ptr[System->Response.Body.len] = source[i];
		System->Response.Body.len++;
		i++;
	} while(source[i] != '\0');

}

//__attribute__((always_inline))
void
_clean1(struct SYSTEM *System) {

	System->Response.Body.len--;
	System->Response.Body.ptr[System->Response.Body.len] = '\0';

}

int
_printf(struct SYSTEM *System, const char * format, ...) {
        
    int result;
    int remain;

    remain = System->Response.Body.allocated - System->Response.Body.len;
    
    va_list args;
    va_start(args, format);
    result = vsprintf(&System->Response.Body.ptr[System->Response.Body.len], format, args);
    va_end(args);
    
    if(result > remain) {
        
        string_grow(&System->Response.Body, System->Response.Body.allocated * 2 + result);
        
        va_start(args, format);
        result = vsprintf(&System->Response.Body.ptr[System->Response.Body.len], format, args);
        va_end(args);
        
    }
    
    System->Response.Body.len += result;
    
    return result;
        
}












//__attribute__((always_inline))
void
_setcookie(struct SYSTEM *System, const char *key, CookieOpt options) {

	string_print(&System->Response.Header2, "Set-Cookie: ");
	string_print(&System->Response.Header2, key);
	string_print(&System->Response.Header2, "=");

	if(options.value != NULL) {
		string_print(&System->Response.Header2, options.value);
	}

	if(options.domain != NULL) {
		string_print(&System->Response.Header2, "; Domain=");
		string_print(&System->Response.Header2, options.domain);
	}

	if(options.path != NULL) {
		string_print(&System->Response.Header2, "; Path=");
		string_print(&System->Response.Header2, options.path);
	} else {
		string_print(&System->Response.Header2, "; Path=/");
	}

	if(options.expire == HTTP_COOKIE_EXPIRE_LAST) {
		string_print(&System->Response.Header2, "; Expires=Sun, 07 Feb 2106 06:28:15 GMT");
	} else if(options.expire == HTTP_COOKIE_EXPIRE_FIRST) {
		string_print(&System->Response.Header2, "; Expires=Thu, 01 Jan 1970 00:00:00 GMT");
	} else {

		/*time_t 		rawtime;
		struct tm *	timeinfo;
		char 		buffer[30];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, 80, "%a, %d-%b-%Y %H:%m:%S GMT", timeinfo);

		_string_print(&System->Response.Header2, "; Expires= ");
		_string_print(&System->Response.Header2, buffer);*/

	}

	if(options.secure != 0) {
		string_print(&System->Response.Header2, "; Secure");
	}

	if(options.httponly != 0) {
		string_print(&System->Response.Header2, "; HttpOnly");
	}

	string_print(&System->Response.Header2, "\r\n");

}


//__attribute__((always_inline))
void
_header(struct SYSTEM *System, int header_type, char *val) {

	switch(header_type) {

		case HTTP_HEADER_CONTENT_TYPE:

			System->Response.Header.Content_Type = System->Response.Header3.ptr;

			string_print(&System->Response.Header3, "Content-Type: ");
			string_print(&System->Response.Header3, val);
			string_print(&System->Response.Header3, "\r\n");

			break;
	}

}











void
string_init(struct String *string, int allocated) {
    string->len         = 0;
    string->allocated   = allocated;
    string->ptr         = malloc(sizeof(char) * allocated);
}

void
string_destroy(struct String *string) {
    string->len         = 0;
    string->allocated   = 0;
    free(string->ptr);
    string->ptr         = NULL;
}

void
string_grow(struct String *string, int allocated) {
    
    char *ptr;
    
    ptr = malloc(sizeof(char) * allocated);
    
    strncpy(ptr, string->ptr, string->len);
    free(string->ptr);
        
    string->allocated   = allocated;
    string->ptr         = ptr;
    
}

void
string_shrink(struct String *string, int allocated) {
    
    char *ptr;
    
    ptr = malloc(sizeof(char) * allocated);
    
    strncpy(ptr, string->ptr, allocated);
    free(string->ptr);
    
    string->allocated   = allocated;
    string->ptr         = ptr;
    
}

void
string_truncate(struct String *string) {
    string->len = 0;
}

void
string_print(struct String *string, const char * source) {

    register int i = 0;

    do {
        string->ptr[string->len] = source[i];
        string->len++;
        i++;
    } while(source[i] != '\0');

}

int
string_printf(struct String *string, const char * format, ...) {

    int result;
    int remain;

    remain = string->allocated - string->len;
    
    va_list args;
    va_start(args, format);
    result = vsnprintf(&string->ptr[string->len], remain, format, args);
    va_end(args);
    
    if(result > remain) {
        
        string_grow(string, string->allocated * 2 + result);
        
        va_start(args, format);
        result = vsprintf(&string->ptr[string->len], format, args);
        va_end(args);
        
    }
    
    string->len += result;
    
    return result;
}
