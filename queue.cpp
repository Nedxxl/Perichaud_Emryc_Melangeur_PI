#include "queue.hpp"

TQueue::TQueue(void) : trig(0)
    {
    queue_init();
    }

TQueue::~TQueue()
    {

    }

void TQueue::queue_init(void)
    {
    head = 0;
    tail = 0;
    }

void TQueue::queue_push(TQueue::node_t * const n,void* const state)
    {
    node_t *_tail;

    n->next = 0;
    n->state = state;

    mtx.take();

    if (!(_tail = tail))
	{
	head = n;
	}
    else
	{
	_tail->next = n;
	}

    tail = n;

    // trig
    trig.release();
    mtx.release();
    }

TQueue::node_t * TQueue::queue_pop(void)
    {
    node_t *_head;

    mtx.take();

    while (! (_head = head))
	{
	mtx.release();
	// wait to trig
	trig.take();
	mtx.take();
	}

    if (!(head = _head->next))
	{
	tail = 0;
	}

    mtx.release();

    return _head;
    }
