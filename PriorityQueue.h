#ifndef __PRIORITYQUEUE_H
#define __PRIORITYQUEUE_H

#include <stddef.h>

typedef struct {
	int sent1;
	int sent2;
	int primaryPriority;
	int secondaryPriority;
} QueueObject;

typedef struct{
	QueueObject **heap;
	int size;
	int count;
}PriorityQueue;

QueueObject *createQueueObject(int sent1, int sent2,
			       int primaryPriority,
			       int secondaryPriority);

PriorityQueue *createPriorityQueue(int size);

int addToQueue(PriorityQueue *Queue, int sent1, int sent2,
		int primaryPriority, int secondaryPriority);

QueueObject *pullFromQueue(PriorityQueue *Queue);

void MaxHeapify(PriorityQueue *Queue, int root);


#endif
