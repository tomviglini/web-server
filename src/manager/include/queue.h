#ifndef QUEUE_H
#define QUEUE_H

struct QUEUE {
	int *index;
	int size;
	int used;
	int last;
	int head;
	int tail;
};

void 	qinit	(struct QUEUE *queue, int size);
int 	qpush	(struct QUEUE *queue, int value);
int 	qpop	(struct QUEUE *queue, int *value);

#endif /* QUEUE_H */
