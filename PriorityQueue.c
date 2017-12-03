#include <stdlib.h>
#include "PriorityQueue.h"

QueueObject *createQueueObject(int sent1, int sent2,
			       int primaryPriority,
			       int secondaryPriority)
{
	return NULL;
}

PriorityQueue *createPriorityQueue(int size)
{
	int queueSizeInBytes = 512 * sizeof(QueueObject *);
	PriorityQueue *Queue = (PriorityQueue*) calloc(1, sizeof(PriorityQueue));

	Queue->heap = (QueueObject **)calloc(512, sizeof(QueueObject *));

	Queue->count = 0;
	Queue->size = 512;

	return Queue;
}

int addToQueue(PriorityQueue *Queue, int sent1, int sent2,
	       int primaryPriority, int secondaryPriority)
{
	QueueObject *ObjectToAdd = createQueueObject(sent1, sent2,
						primaryPriority,
						secondaryPriority);
	int index = Queue->count++;
	int parent;
	for (;index != 0; index = parent){
		parent = (index - 1) / 2;
		if (Queue->heap[parent]->primaryPriority < primaryPriority){
			break;
		} else if (Queue->heap[parent]->primaryPriority == primaryPriority){
			if (Queue->heap[parent]->secondaryPriority <= secondaryPriority) {
				break;
			}
		}
		Queue->heap[index] = Queue->heap[parent];
	}
	Queue->heap[index] = ObjectToAdd;
}

QueueObject *removeFromQueue(PriorityQueue *Queue)
{
	QueueObject *ObjectToReturn = Queue->heap[0];
	Queue->count--;
	QueueObject *temp = Queue->heap[Queue->count];

	Queue->heap[0] = temp;
	MaxHeapify(Queue, 0);
	return ObjectToReturn;

}

void MaxHeapify(PriorityQueue *Queue, int root)
{
	int left, right, smallest;
	QueueObject *temp;
	left = 2*(root) + 1;
	right = root + 1;

	smallest = root;

	if (left <= Queue->count && Queue->heap[left] < Queue->heap[smallest]) {
		smallest = left;
	}
	if (right <= Queue->count && Queue->heap[right] < Queue->heap[smallest]) {
		smallest = right;
	}
	if (smallest != root) {
		temp = Queue->heap[root];
		Queue->heap[root] = Queue->heap[smallest];
		Queue->heap[smallest] = temp;
		MaxHeapify(Queue, smallest);
	}
	return;
}
