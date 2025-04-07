#ifndef HEADER_FILE
#define HEADER_FILE



struct element {
	// Define the struct yourself
	char operation[10];
  	int order;
	int num_account;
  	int num_account2;
	int amount;
};

typedef struct queue {
	// Define the struct yourself
	int size; // it is fixed
	int head; // actual position
	int tail; // final position
	int contains; // # elements in the queue currently
	struct element *e_list; // list with the elements
	
}queue;

queue* queue_init (int size);
int queue_destroy (queue *q);
int queue_put (queue *q, struct element* elem);
struct element * queue_get(queue *q);
int queue_empty (queue *q);
int queue_full(queue *q);

#endif