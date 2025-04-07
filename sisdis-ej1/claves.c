#include "include/claves.h"

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

List tuples = NULL;

int destroy() {
    if (tuples == NULL) return 0;      
    
    struct Node *next;
    
    while (tuples != NULL) {
        next = tuples->next;
        free(tuples);
        tuples = next;
    }
    
    tuples = NULL;

    return 0;
}

int set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    if (exist(key) == 1) return -1;

    if (N_value2 < 1 || N_value2 > 32) return -1;

    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if (new_node == NULL) return -1;

    new_node->key = key;
    strncpy(new_node->value1, value1, sizeof(new_node->value1));

    new_node->N_value2 = N_value2;
    memcpy(new_node->value2, V_value2, N_value2 * sizeof(double));

    new_node->value3 = value3;
    new_node->next = NULL;

    return add_node(new_node);
}

int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3) {
    if (tuples == NULL) return -1;     

    struct Node *node = search_node(key);
    if (node != NULL) { 
        strncpy(value1, node->value1, sizeof(node->value1));

        *N_value2 = node->N_value2;
        memcpy(V_value2, node->value2, node->N_value2 * sizeof(double));

        *value3 = node->value3;
        return 0;
    }

    return -1;      
}

int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    if (N_value2 < 1 || N_value2 > 32) return -1;

    struct Node *node = search_node(key);
    if (node != NULL) {
        memset(node->value1, 0, sizeof(node->value1));
        memset(node->value2, 0, N_value2 * sizeof(double));
        
        strncpy(node->value1, value1, sizeof(node->value1));

        node->N_value2 = N_value2;
        memcpy(node->value2, V_value2, N_value2 * sizeof(double));   

        node->value3 = value3;

        return 0; 
    }

    return -1;
} 

int delete_key(int key) {
    if (tuples == NULL) return -1;

    struct Node *node = search_node(key);
    if (node == NULL) return -1;

    if (tuples == node) {
        tuples = tuples->next;
        free(node);
        return 0;
    }
    
    struct Node *prev = tuples;
    while (prev->next != NULL && prev->next != node)
        prev = prev->next;

    if (prev->next == NULL) return -1; 

    prev->next = node->next;
    free(node);
    return 0;
}

int exist(int key) {
    return (search_node(key) != NULL) ? 1 : 0;
}

struct Node* search_node(int key) {
    struct Node *aux = tuples; 
    while (aux != NULL) {
        if (aux->key == key) {  
            return aux;
        }
        aux = aux->next;
    }
    return NULL;  
}

int add_node(struct Node *new_node) {
    if (tuples == NULL) {  
        tuples = new_node;  
    } else {
        new_node->next = tuples;    
        tuples = new_node;  
    }
    return 0;
}
