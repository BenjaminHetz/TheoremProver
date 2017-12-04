#include <stdlib.h>
#include <stdio.h>

#include "PriorityQueue.h"

QueueObject *createQueueObject(int sent1, int sent2,
			       int primaryPriority,
			       int secondaryPriority)
{
	QueueObject *Object = (QueueObject *)malloc(sizeof(QueueObject));
	Object->sent1 = sent1;
	Object->sent2 = sent2;
	Object->primaryPriority = primaryPriority;
	Object->secondaryPriority = secondaryPriority;
	fprintf(stderr, "Object looks like:\nsent1:\t%d\nsent2:\t%d\nPriority1:\t%d\nPriority2:\t%d\n", sent1, sent2, primaryPriority, secondaryPriority);
	return Object;
}

PriorityQueue *createPriorityQueue()
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
	if (Queue->count >= Queue->size){
		fprintf(stderr, "Count exceeded size, realloc needed\n");
		QueueObject **temp = realloc(Queue->heap, (Queue->size) * 2);
		if (temp == NULL){
			fprintf(stderr, "Realloc failed\n");
			exit(-1);
		}
		Queue->heap = temp;
		Queue->size = Queue->size * 2;
	}
	QueueObject *ObjectToAdd = createQueueObject(sent1, sent2,
						     primaryPriority,
						     secondaryPriority);
	int index = Queue->count++;
	int parent;
	for (;index != 0; index = parent){
		parent = (index - 1) / 2;
//		fprintf(stderr, "Printing heap parent: %d\n", parent);
//		fprintf(stderr, "%d\n",Queue->heap[parent]->primaryPriority);
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

QueueObject *pullFromQueue(PriorityQueue *Queue)
{
	if (Queue->count == 0) {
		return NULL;
	}
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
