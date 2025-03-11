#ifndef SERVICIOS_SERVER_H
#define SERVICIOS_SERVER_H

// Para la lista enlazada
struct Node {
    int clave;
    char valor1[256];
    int n;
    double valor2[32];
    struct Node *next;
};

typedef struct Node * List;

// Funciones en el lado del servidor
int init_server (List *tuplas);
int set_value_server(List *tuplas, int key, char *value1, int N_value2, double *V_value2);
int get_value_server(List *tuplas, int key, char *value1, int *N_value2, double *V_value2);
int modify_value_server(List *tuplas, int key, char *value1, int N_value2, double *V_value2);
int delete_key_server(List *tuplas, int key);
int exist_server(List *tuplas, int key);
struct Node *buscarNodo(List *tuplas, int key);
int insertarNodo(List *tuplas, struct Node *new_node);

// Debugg
int printList(List l); 
void printNode(struct Node *node); 

#endif // SERVICIOS_SERVER_H