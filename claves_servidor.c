#include "include/claves_servidor.h"

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

int destroy_server(List *tuples) {
    if (*tuples == NULL) return 0;      

    struct Node *next;
    
    while ((*tuples)->next != NULL) {
        next = (*tuples)->next;
        free(*tuples);
        *tuples = next;
    }
    
    free(*tuples);
    *tuples = NULL;

    return 0;
}

int set_value_server(List *tuples, int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    if (exist_server(tuples, key) == 1) return -1;

    if (N_value2 < 1 || N_value2 > 32) return -1;

    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if (new_node == NULL) return -1;

    new_node->key = key;

    strncpy(new_node->value1, value1, sizeof(new_node->value1));

    new_node->N_value2 = N_value2;
    memcpy(new_node->value2, V_value2, N_value2 * sizeof(double));

    new_node->value3 = value3;

    return add_node(tuples, new_node);
}

int get_value_server(List *tuples, int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3) {
    if (*tuples == NULL) return -1;     

    struct Node *node = search_node(tuples, key);

    if (node != NULL) { 
        strncpy(value1, node->value1, sizeof(value1));

        *N_value2 = node->N_value2;
        memcpy(V_value2, node->value2, node->N_value2 * sizeof(double));

        *value3 = node->value3;

        return 0;
    }

    return -1;      
}

int modify_value_server(List *tuples, int key, char *value1, int N_value2, double *V_value2, struct Coord *value3) {
    if (N_value2 < 1 || N_value2 > 32) return -1;

    struct Node *node = search_node(tuples, key);

    if (node != NULL) {
        bzero(node->value1, sizeof(node->value1));
        bzero(node->value2, N_value2 * sizeof(double));
        
        strncpy(node->value1, value1, sizeof(node->value1));

        node->N_value2 = N_value2;
        memcpy(node->value2, V_value2, N_value2 * sizeof(double));   

        node->value3 = value3;

        return 0; 
    }

    return -1;
} 

int delete_key_server(List *tuples, int key) {
    struct Node *node = search_node(tuples, key);

    if (node == NULL) return -1;

    if (*tuples == node && (*tuples)->next == NULL) {
        free(*tuples);
        *tuples = NULL; 
        return 0;
    }
    
    if (*tuples == node) {
        *tuples = (*tuples)->next; 
        free(node);
        return 0;
    }
    
    struct Node *prev = *tuples;
    while (prev->next != node)
        prev = prev->next;

    prev->next = node->next;
    free(node);
    return 0;
}

int exist_server(List *tuples, int key){
    if (search_node(tuples, key) != NULL) return 1;
    
    return 0;   
}

struct Node* search_node(List *tuples, int key) {
    List aux = *tuples; 
    while (aux != NULL) {
        if (aux->key == key) {  
            return aux;
        } else {
            aux = aux->next;
        }
    }

    return NULL;  
}

int add_node(List *tuples, struct Node *new_node) {
    if (*tuples == NULL) {  
        *tuples = new_node;  
   
    } else {
        new_node->next = *tuples;    
        *tuples = new_node;  
    }

    return 0;
}

// int printList(List l){
//     struct Node *aux = l;

//     while(aux != NULL)
//     {
//         printNode(aux);	
//         aux = aux->next;
//     }
//     return 0;
// }

// void printNode(struct Node *node) {
//     printf("nodeKey: %d ------------\N_value2", node->key);
//     printf("v1: %s\N_value2", node->value1);
//     printf("N_value2: %d\N_value2", node->N_value2);
//     printf("v2-lista:\N_value2");
//     for (int i = 0; i < node->N_value2; i++)
//     {
//         printf("\t· %lf\N_value2", node->value2[i]);
//     }
// }
