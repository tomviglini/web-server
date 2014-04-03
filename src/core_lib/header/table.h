#ifndef TABLE_H
#define TABLE_H

#include "list.h"

struct TABLE {
	struct LIST apath;
	struct LIST rpath;
	unsigned int used;
};

void table_create	(struct TABLE **table);
void table_free		(struct TABLE *table);
void table_init		(struct TABLE *table);

#endif /* TABLE_H */
