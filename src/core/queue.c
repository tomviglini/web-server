#include <stdlib.h>
#include <pthread.h>

#include "queue.h"

__attribute__((always_inline))
void
qinit(struct QUEUE *queue, int size) {
	queue->index = malloc(sizeof(int) * size);
	queue->size = size;
	queue->last = size - 1;
	queue->used = 0;
	queue->head = 0;
	queue->tail = 0;
}

__attribute__((always_inline))
int
qpush(struct QUEUE *queue, int value) {

	if(queue->used == queue->size) {
		return 1;
	}

	queue->used++;

	if(queue->tail == queue->last) {
		queue->index[queue->tail] = value;
		queue->tail = 0;
	} else {
		queue->index[queue->tail] = value;
		queue->tail++;
	}

	return 0;
}

__attribute__((always_inline))
int
qpop(struct QUEUE *queue, int *value) {

	if(queue->used == 0) {
		return 1;
	}

	queue->used--;
	*value = queue->index[queue->head];

	if(queue->head == queue->last) {
		queue->head = 0;
	} else {
		queue->head++;
	}

	return 0;
}
