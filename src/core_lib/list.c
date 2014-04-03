#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "header/list.h"
#include "header/file.h"






__attribute__((always_inline))
void *
bsearch(
	register const void *key,
	const void *base0,
	size_t nmemb,
	register size_t size,
	register int (*compar)(const void *, const void *)
){
	register const char *base = base0;
	register size_t lim;
	register int cmp;
	register const void *p;

	for (lim = nmemb; lim != 0; lim >>= 1) {
		p = base + (lim >> 1) * size;

		cmp = (*compar)(key, p);

		if (cmp == 0)
			return ((void *)p);
		if (cmp > 0) {	/* key > p: move right */
			base = (char *)p + size;
			lim--;
		}		/* else move left */
	}
	return (NULL);
}








int
searchcompare(const void *node1, const void *node2) {

	return strcmp(
		(const char *) node1,
		(*(struct RECORD * const *) node2)->key
	);


	/*if(
			*(unsigned long *) node1 ==
			((unsigned long)(*(struct RECORD * const *) node2)->key)

	) 	return 0;

	if(
			*(unsigned long *) node1 >
			((unsigned long)(*(struct RECORD * const *) node2)->key)

	) 	return 1;*/


}

int
sortcompare(const void *node1, const void *node2) {

	return strcmp(
		(*(struct RECORD * const *) node1)->key,
		(*(struct RECORD * const *) node2)->key
	);


	/*if(
			((unsigned long)(*(struct RECORD * const *) node1)->key) ==
			((unsigned long)(*(struct RECORD * const *) node2)->key)

	) 	return 0;

	if(
			((unsigned long)(*(struct RECORD * const *) node1)->key) >
			((unsigned long)(*(struct RECORD * const *) node2)->key)

	) 	return 1;*/


																								return -1;

}




__attribute__((always_inline))
void
linit(struct LIST *list) {
	list->size = 0;
	list->record = NULL;
}

__attribute__((always_inline))
void
lfree(struct LIST *list, int(* free_val)(void *)) {

	int i;

	for(i = 0; i < list->size; i++) {
		free(list->record[i]->key);
		if(free_val != NULL) {
			free_val(list->record[i]->val);
		}
	}

	free(list->record);

}

__attribute__((always_inline))
void
lset(struct LIST *list, struct RECORD **record, const char *key) {

	if(list->size == 0) {

		list->record = malloc(sizeof(struct RECORD *));

		list->record[0] = malloc(sizeof(struct RECORD));

		list->record[0]->key = malloc(sizeof(char) * (strlen(key) + 1));
		strcpy(list->record[0]->key, key);

		*record = list->record[0];

		list->size = 1;

	} else {

		int i;
		struct RECORD **tmp;

		tmp = malloc(sizeof(struct RECORD *) * (list->size + 1));

		for(i = 0; i < list->size; i++) {
			tmp[i] = list->record[i];
		}

		tmp[list->size] = malloc(sizeof(struct RECORD));

		tmp[list->size]->key = malloc(sizeof(char) * (strlen(key) + 1));

		strcpy(tmp[list->size]->key, key);

		*record = tmp[list->size];

		free(list->record);

		list->record = tmp;

		list->size++;

		qsort(list->record, list->size, sizeof(list->record[0]), sortcompare);

	}

}


__attribute__((always_inline))
void
lget(struct LIST *list, struct RECORD **record, const char *key) {

	if(list->size == 0) {
		*record = NULL;
		return;
	}

	*record = bsearch(key, list->record, list->size, sizeof(list->record[0]), searchcompare);

	if(*record != NULL) {
		*record = *(struct RECORD **) *record;
	}
}
