#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#include "include/servicios_server.h"
#include "include/error.h"

struct Node *buscarNodo(List *tuplas, int key) {
    List aux = *tuplas; // Utilizamos un puntero auxiliar para recorrer la lista enlazada
    while (aux != NULL) {
        if (aux->clave == key) {  // Hemos encontrado el nodo
            return aux;
        } else {
            aux = aux->next;
        }
    }
    return NULL;  // No se ha encontrado
}

int insertarNodo(List *tuplas, struct Node *new_node) {
    if (*tuplas == NULL) {  // Si la lista está vacía
        *tuplas = new_node;  // Modificamos cabecera
    } else {
        new_node->next = *tuplas;    // Actualizamos el next
        *tuplas = new_node;  // Actualizamos cabecera
    }
    return 0;
}

int init_server(List *tuplas) {
    if (*tuplas == NULL) return 0;      // Lista vacia

    struct Node *siguiente;

    // Bucle para liberar la memoria de cada nodo
    while ((*tuplas)->next != NULL) {
        siguiente = (*tuplas)->next;
        free(*tuplas);
        *tuplas = siguiente;
    }

    // Libera el último nodo y establece la lista en NULL
    free(*tuplas);
    *tuplas = NULL;

    return 0;
}

int set_value_server(List *tuplas, int key, char *value1, int N_value2, double *V_value2) {
    if (exist_server(tuplas, key) == 1) {       // Ya existe un nodo con la misma clave
        return -1;
    }

    if (N_value2 < 1 || N_value2 > 32)      // Si N esta fuera de rango...
        return -1;

    // Creamos el nodo que queremos insertar
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if (new_node == NULL) return -1;

    new_node->clave = key;
    strncpy(new_node->valor1, value1, sizeof(new_node->valor1));
    new_node->n = N_value2;
    memcpy(new_node->valor2, V_value2, N_value2 * sizeof(double));

    return insertarNodo(tuplas, new_node);
}

int get_value_server(List *tuplas, int key, char *value1, int *N_value2, double *V_value2) {
    if (*tuplas == NULL) return -1;     // Lista vacia

    struct Node *found_node = buscarNodo(tuplas, key);

    // Copiamos los datos
    if (found_node != NULL) { 
        strncpy(value1, found_node->valor1, sizeof(value1));
        *N_value2 = found_node->n;
        memcpy(V_value2, found_node->valor2, found_node->n * sizeof(double));
        return 0;
    }

    return -1;      // No se ha encontrado el nodo
}

int modify_value_server(List *tuplas, int key, char *value1, int N_value2, double *V_value2) {
    if (N_value2 < 1 || N_value2 > 32)      // Si N esta fuera de rango...
        return -1;

    // Busca el nodo con la clave proporcionada
    struct Node *nodo_a_modificar = buscarNodo(tuplas, key);

    if (nodo_a_modificar != NULL) {
        // Nos aseguramos de limpiar los campos:
        bzero(nodo_a_modificar->valor1, sizeof(nodo_a_modificar->valor1));
        bzero(nodo_a_modificar->valor2, N_value2 * sizeof(double));

        // Modifica los valores del nodo encontrado
        strncpy(nodo_a_modificar->valor1, value1, sizeof(nodo_a_modificar->valor1));
        nodo_a_modificar->n = N_value2;
        memcpy(nodo_a_modificar->valor2, V_value2, N_value2 * sizeof(double));   
        return 0; 
    }

    return -1;
} 

int delete_key_server(List *tuplas, int key) {
    // Busca el nodo con la clave proporcionada
    struct Node *nodo_a_eliminar = buscarNodo(tuplas, key);

    // Si el nodo no se encuentra, devuelve -1
    if (nodo_a_eliminar == NULL)
        return -1;

    // Si el nodo a eliminar es el primer y único nodo de la lista
    if (*tuplas == nodo_a_eliminar && (*tuplas)->next == NULL) {
        free(*tuplas);
        *tuplas = NULL; // Actualiza el puntero inicial a NULL
        return 0;
    }

    // Si el nodo a eliminar es el primer nodo de la lista
    if (*tuplas == nodo_a_eliminar) {
        *tuplas = (*tuplas)->next; // Actualiza el puntero inicial
        free(nodo_a_eliminar);
        return 0;
    }

    // Busca el nodo anterior al nodo a eliminar
    struct Node *anterior = *tuplas;
    while (anterior->next != nodo_a_eliminar)
        anterior = anterior->next;

    // Elimina el nodo de la lista
    anterior->next = nodo_a_eliminar->next;
    free(nodo_a_eliminar);
    return 0;
}

int exist_server(List *tuplas, int key){
    if (buscarNodo(tuplas, key) != NULL) {
        return 1;
    } else {
        return 0;   // No se ha encontrado
    }
    return -1;  // Error 
}

// Funciones para comprobar el estado de la lista
int printList(List l){
    struct Node *aux = l;

    while(aux != NULL)
    {
        printNode(aux);	
        aux = aux->next;
    }
    return 0;
}

void printNode(struct Node *node) {
    printf("nodeKey: %d ------------\n", node->clave);
    printf("v1: %s\n", node->valor1);
    printf("N: %d\n", node->n);
    printf("v2-lista:\n");
    for (int i = 0; i < node->n; i++)
    {
        printf("\t· %lf\n", node->valor2[i]);
    }
}
