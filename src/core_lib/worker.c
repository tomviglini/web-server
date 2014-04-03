#include "header/worker.h"

__attribute__((always_inline))
void
worker_init(struct WORKER *worker, int size) {
	int i;

	worker->record = malloc(sizeof(struct WORKER_RECORD) * size);

	for(i = 0; i < size; i++) {
		/*thread->record[i].flags = 0;*/

		worker->record[i].close_while_busy = 0;
	}
}
