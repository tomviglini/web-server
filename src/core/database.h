#ifndef DATABASE_H
#define DATABASE_H

#include "../core_lib/header/table.h"

struct DATABASE {
	//struct TABLE *current;
	//struct TABLE *old;


	int current;
	int used[2];
};

void database_init(struct DATABASE *database);

#endif /* DATABASE_H */
