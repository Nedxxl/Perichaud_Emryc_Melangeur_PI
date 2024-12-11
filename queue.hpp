#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "sem.hpp"
#include "mutex.hpp"

class TQueue
    {
    public:
	struct node_t
	    {
	    node_t * next;
	    void * state;
	    };
    private:
	node_t * head;
	node_t * tail;
	TMutex mtx;
	TSemaphore trig;

    public:
    TQueue(void);
    ~TQueue();

    void queue_init(void);
    void queue_push(node_t * const n,void* const state);
    node_t * queue_pop(void);

    };

#endif //QUEUE_HPP
