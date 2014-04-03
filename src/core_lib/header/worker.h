#ifndef WORKER_H
#define WORKER_H

/*enum WORKER_STATE {
    CLOSE_WHILE_BUSY = 1
};*/

/*const int WORKER_RECORD_CLOSE_WHILE_BUSY = 1;*/
#define WORKER_RECORD_CLOSE_WHILE_BUSY 1

struct WORKER_RECORD {
	/*unsigned int flags;*/

	/* test */
	unsigned int close_while_busy;
};

struct WORKER {
	struct WORKER_RECORD *record;
};

void worker_init(struct WORKER *worker, int size);

#endif /* WORKER_H */
