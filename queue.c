// OS-P3 2022-2023

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "queue.h"

// To create a queue
queue *queue_init(int size)
{
	// Making the gap for the queue and its list, and initializing the attributes
	queue *q = (queue *)malloc(sizeof(queue));
	q->size = size;
	q->head = 0;
	q->tail = 0;
	q->contains = 0;
	q->e_list = malloc(q->size * sizeof(struct element));
	return q;
}

// To Enqueue an element
int queue_put(queue *q, struct element *x)
{
	// If the queue isn't full
	if (queue_full(q) == 0)
	{	
		q->e_list[(q->tail) % q->size] = *x;
		q->tail++;
		q->contains++;
		return 0;
	}
	return -1;
}

// To Dequeue an element.
struct element *queue_get(queue *q)
{
	// Creating the element that will be returned
	struct element *element = malloc(sizeof(struct element));
	// If the queue is not empty
	if (queue_empty(q) == 0)
	{
		*element = q->e_list[q->head];
		q->head = (q->head + 1) % q->size;
		q->contains--;
	}
	return element;
}

// To check queue state
int queue_empty(queue *q)
{
	// Checks if it contains something
	if (q->contains == 0)
	{	
		return 1;
	}
	return 0;
}

int queue_full(queue *q)
{
	// Checks if what it contains is equal to the size
	if (q->contains == q->size)
	{
		return 1;
	}
	return 0;
}

// To destroy the queue and free the resources
int queue_destroy(queue *q)
{
	// free the elements list
	free(q->e_list);
	// free the space of the queue
	free(q);
	return 0;
}