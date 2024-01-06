// OS-P3 2022-2023

#include "queue.h"
#include <fcntl.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * Entry point
 * @param argc
 * @param argv
 * @return
 */

queue *q;
char **list_clients_ops;
int client_numop = 0;
int bank_numop = 0;
int global_balance = 0;
int *account_balance;
int number_of_operations;

pthread_mutex_t mutex; // controls accesses to shared resources
pthread_mutex_t atm_mutex; // controls a unique access of an atm
pthread_mutex_t worker_mutex; // controls a unique access of a worker
pthread_cond_t no_full;  // controls if the queue is full
pthread_cond_t no_empty; // controls if the queue is empty

void atm();
void worker();

int main(int argc, const char *argv[]) {
  // Check if there are 5 arguments
  if(argc < 6){
    return -1;
  }

  // initializes the mutex and conditions
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&atm_mutex, NULL);
  pthread_mutex_init(&worker_mutex, NULL);
  pthread_cond_init(&no_full, NULL);
  pthread_cond_init(&no_empty, NULL);

  // Storing the arguments in variables
  int n_atm = atoi(argv[2]);
  int n_workers = atoi(argv[3]);
  int max_accounts = atoi(argv[4]);
  int buffsize = atoi(argv[5]);


  if (n_atm < 0 || n_workers < 0 || max_accounts < 0 || buffsize < 0) {
    perror("Wrong arguments.");
    return -1;
  }

  // Creating space for the balance of each account
  account_balance = (int *)malloc(max_accounts * sizeof(int));

  // Opening the file
  FILE *fd = fopen(argv[1], "r");
  if (fd == NULL) {
    perror("Error when opening the file");
    return -1;
  }

  // Getting how many operations are in the file
  if (fscanf(fd, "%d\n", &number_of_operations) < 0) {
    perror("Error when extracting data from the file");
    return -1;
  }

  // Checks that the number of operations is in range
  if (number_of_operations < 1 || number_of_operations > 200) {
    perror("Invalid number of operations");
    return -1;
  }

  // Creating space for the list of operations
  list_clients_ops = (char **)malloc(number_of_operations * sizeof(char *));

  // For each row, allocate memory for ncols elements
  for (int i = 0; i < number_of_operations; ++i) {
    list_clients_ops[i] = (char *)malloc(64 * sizeof(char));
  }
  
  // counter for number of accounts
  // counter2 for the real number of operations
  int counter = 0, counter2 = 0;
  // auxiliar variables
  char data[100];
  int i = 0;

  // Reading lines until the end of the file
  while (fgets(list_clients_ops[i], 100, fd)) {
    // Counting the # of accounts created
    sscanf(list_clients_ops[i], "%s", data);
    counter2++;  
    if (strcmp(data, "CREATE") == 0) {
      counter++;
    }
    i++;
  }

  if (counter2 != number_of_operations) {
    perror("There are more operations than expected");
    return -1;
  }
  if (counter > max_accounts) {
    perror("There are more accounts than expected");
    return -1;
  }
  // Closes the file
  fclose(fd);

  // Initialize the queue
  q = queue_init(buffsize);
  // Creation of a matrix of all the threads
  int n_threads = n_atm + n_workers;
  pthread_t thrs[n_threads];
  for (int i = 0; i < n_threads; i++) {
    // ATM threads
    if (i < n_atm) {
      if (pthread_create(&thrs[i], NULL, (void *)atm, NULL) < 0) {
        perror("Error when creating atms' threads");
        return -1;
      }
    } else {
      // WORKERS threads
      if (pthread_create(&thrs[i], NULL, (void *)worker, NULL) < 0) {
        perror("Error when creating atms' threads");
        return -1;
      }
    }
  }

  // Waiting until all threads have finished
  for (int i = 0; i < n_threads; i++) {
    if (pthread_join(thrs[i], NULL) != 0) {
      perror("Error when joining ATMs' threads");
      return -1;
    }
  }

  // Destroying any mutex or condition
  pthread_mutex_destroy(&mutex);
  pthread_mutex_destroy(&atm_mutex);
  pthread_mutex_destroy(&worker_mutex);
  pthread_cond_destroy(&no_full);
  pthread_cond_destroy(&no_empty);

  // Free the previous space taken
  free(list_clients_ops);
  free(account_balance);

  // Deleting the queue and its elements
  queue_destroy(q);
  return 0;
}

void atm(void) {
  // It is done only while there are operations available
  while (client_numop < number_of_operations) {
    // Only one ATM at each time
    pthread_mutex_lock(&atm_mutex);
    // Checks if other thread has done an operation while waiting
    if (client_numop >= number_of_operations) {
      pthread_mutex_unlock(&atm_mutex);
      break;
    }
    // Locks the shared resources
    pthread_mutex_lock(&mutex);
    // If full: waits until it receives a signal that it isn't full
    while (queue_full(q) == 1) {
      pthread_cond_wait(&no_full, &mutex);
    }

    // Creating an element to put it
    struct element actual_element;
    actual_element.order = client_numop + 1;

    // Differencing the operation procedure depending on the name
    sscanf(list_clients_ops[client_numop], "%s", actual_element.operation);
    if (strcmp(actual_element.operation, "CREATE") == 0 ||
        strcmp(actual_element.operation, "BALANCE") == 0) {
  
      sscanf(list_clients_ops[client_numop], "%s %d", actual_element.operation,
             &actual_element.num_account);
      
    } else if (strcmp(actual_element.operation, "DEPOSIT") == 0 ||
               strcmp(actual_element.operation, "WITHDRAW") == 0) {
     
      sscanf(list_clients_ops[client_numop], "%s %d %d",
             actual_element.operation, &actual_element.num_account, &actual_element.amount);
      
    } else if (strcmp(actual_element.operation, "TRANSFER") == 0) {
    
      sscanf(list_clients_ops[client_numop], "%s %d %d %d",
             actual_element.operation, &actual_element.num_account, &actual_element.num_account2, &actual_element.amount);
      
    } else {
      perror("The name of the operation is not valid");
    }

    // Inserting the element
    queue_put(q, &actual_element);
    // Another operation done
    client_numop++;
    // Unlocking the condition and both mutex
    pthread_cond_signal(&no_empty);
    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&atm_mutex);
  }
  pthread_exit(0);
}

void worker() {
  // It is done only while there are operations available
  while (bank_numop < number_of_operations) {
    // Only one worker at each time
    pthread_mutex_lock(&worker_mutex);
    // Checks if other thread has done an operation while waiting
    if (bank_numop >= number_of_operations) {
      pthread_mutex_unlock(&worker_mutex);
      break;
    }
    // Locks the shared resources
    pthread_mutex_lock(&mutex);
    // If empty: waits until it receives a signal that it isn't empty
    while (queue_empty(q) == 1) {
      pthread_cond_wait(&no_empty, &mutex);
    }
    // Getting an element from the queue
    struct element actual_element;
    actual_element = *queue_get(q);
    // Printing the number and operation
    printf("%d %s ", actual_element.order, actual_element.operation);
    // Printing depending on the operation and making different calculations
    if (strcmp(actual_element.operation, "CREATE") == 0) {
      // When creating an account, the starting balance is 0
      account_balance[actual_element.num_account - 1] = 0;
      printf("%d BALANCE=%d ", actual_element.num_account,
             account_balance[actual_element.num_account - 1]);

    } else if (strcmp(actual_element.operation, "DEPOSIT") == 0) {
      account_balance[actual_element.num_account - 1] += actual_element.amount;
      global_balance += actual_element.amount;
      printf("%d %d BALANCE=%d ", actual_element.num_account,
             actual_element.amount,
             account_balance[actual_element.num_account - 1]);

    } else if (strcmp(actual_element.operation, "WITHDRAW") == 0) {

      account_balance[actual_element.num_account - 1] -= actual_element.amount;
      global_balance -= actual_element.amount;
      printf("%d %d BALANCE=%d ", actual_element.num_account,
             actual_element.amount,
             account_balance[actual_element.num_account - 1]);

    } else if (strcmp(actual_element.operation, "TRANSFER") == 0) {
      account_balance[actual_element.num_account - 1] -= actual_element.amount;
      account_balance[actual_element.num_account2 - 1] += actual_element.amount;
      printf("%d %d %d BALANCE=%d ", actual_element.num_account,
             actual_element.num_account2, actual_element.amount,
             account_balance[actual_element.num_account2 - 1]);

    } else {
      printf("%d BALANCE=%d ", actual_element.num_account,
             account_balance[actual_element.num_account - 1]);
    }
    // Another operation done
    bank_numop++;
    // Printing the global balance
    printf("TOTAL=%d\n", global_balance);
    // Unlocking the condition and both mutex
    pthread_cond_signal(&no_full);
    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&worker_mutex);
  }

  pthread_exit(0);
}