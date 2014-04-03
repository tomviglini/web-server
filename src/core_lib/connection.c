#include "header/connection.h"

__attribute__((always_inline))
void
connection_init(struct CONNECTION *connection, int bucket_size) {
	int i;

	connection->record = malloc(sizeof(struct CONNECTION_RECORD) * bucket_size);
	connection->size = bucket_size;

	for(i = 0; i < bucket_size; i++) {
		/*connection->record[i].flags = 0;*/
		connection->record[i].active = 0;
		connection->record[i].busy = 0;
		connection->record[i].pending = 0;
	}
}
