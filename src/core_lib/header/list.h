#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct RECORD {
	char *key;
	void *val;
};

struct LIST {
	struct RECORD **record;
	int size;
};

int sortcompare		(const void *node1, const void *node2);
int searchcompare	(const void *node1, const void *node2);

void linit			(struct LIST *list);
void lset			(struct LIST *list, struct RECORD **record, const char *key);
void lget			(struct LIST *list, struct RECORD **record, const char *key);
void lfree			(struct LIST *list, int(* free_val)(void *));

#endif /* LIST_H */
