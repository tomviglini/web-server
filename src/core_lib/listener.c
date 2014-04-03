#include <stdlib.h>

#include "header/listener.h"

__attribute__((always_inline))
void
listener_init(struct LISTENER *listener, int bucket_size) {
	int i;

	listener->record = malloc(sizeof(struct LISTENER_RECORD) * bucket_size);
	listener->size = bucket_size;

	for(i = 0; i < bucket_size; i++) {
		/*listener->record[i].flags = 0;*/
		listener->record[i].active = 0;
	}
}
