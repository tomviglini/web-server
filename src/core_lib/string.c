#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "header/string.h"
#include "header/system.h"



struct SYSTEM *
_system(char *argv[]) {
	return (struct SYSTEM *) argv[1];
}

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
