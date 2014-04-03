#include "database.h"

__attribute__((always_inline))
void
database_init(struct DATABASE *database) {
	database->current = 0;
	database->used[0] = 0;
	database->used[1] = 0;
}
