#ifndef STRING_H
#define STRING_H

#include "header/system.h"

struct String {
    char *ptr;
    int allocated;
    int len;
};



struct SYSTEM *_system(char *argv[]);
void _print     (struct SYSTEM *System, const char *source);
void _clean1    (struct SYSTEM *System);
int _printf     (struct SYSTEM *System, const char * format, ...);







void 	string_init	(struct String *string, int allocated);
void    string_destroy  (struct String *string);
void    string_grow     (struct String *string, int allocated);
void    string_shrink   (struct String *string, int allocated);
void    string_truncate (struct String *string);
void 	string_print	(struct String *string, const char * source);
int 	string_printf	(struct String *string, const char * format, ...);

#endif /* STRING_H */
