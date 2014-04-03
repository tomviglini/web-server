#include "header/table.h"
#include "header/list.h"
#include "header/file.h"

__attribute__((always_inline))
void
table_create(struct TABLE **table) {
	*table = malloc(sizeof(struct TABLE));
}

__attribute__((always_inline))
void
table_free(struct TABLE *table) {

	lfree(&table->apath, path_free);
	lfree(&table->rpath, NULL);

	free(table);
}

__attribute__((always_inline))
void
table_init(struct TABLE *table) {
	linit(&table->apath);
	linit(&table->rpath);
	table->used = 0;
}
