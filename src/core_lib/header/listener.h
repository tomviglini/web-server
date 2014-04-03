#ifndef LISTENER_H
#define LISTENER_H

#include "list.h"

struct LISTENER_RECORD {
	struct LIST host;
	unsigned int active;
};

struct LISTENER {
	struct LISTENER_RECORD *record;
	int size;
};

void listener_init(struct LISTENER *listener, int bucket_size);

#endif /* LISTENER_H */
