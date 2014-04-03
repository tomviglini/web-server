#ifndef CONNECTION_H
#define CONNECTION_H

struct CONNECTION_RECORD {
    /*unsigned int 	flags;*/
    int                 listener;
    unsigned int 	timeout;
    int			worker;

    /* test */
    unsigned int 	active;
    unsigned int	busy;
    unsigned int 	pending;
};

struct CONNECTION {
    struct CONNECTION_RECORD *record;
    int size;
};

void connection_init(struct CONNECTION *connection, int bucket_size);

#endif /* CONNECTION_H */
