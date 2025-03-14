#ifndef SERVICIOS_SERVER_H
#define SERVICIOS_SERVER_H

struct Node { 
    int key;
    char value1[256];
    int N_value2;
    double value2[32];
    struct Coord value3;
    struct Node *next;
};

typedef struct Node *List;

int destroy_server(List *tuples);
int set_value_server(List *tuples, int key, char *value1, int N_value2, double *V_value2, struct Coord value3);
int get_value_server(List *tuples, int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3);
int modify_value_server(List *tuples, int key, char *value1, int N_value2, double *V_value2, struct Coord value3);
int delete_key_server(List *tuples, int key);
int exist_server(List *tuples, int key);
struct Node* get_node(List *tuples, int key);
int add_node(List *tuples, struct Node *new_node);

// Debugg
// int printList(List l); 
// void printNode(struct Node *node); 

#endif // SERVICIOS_SERVER_H