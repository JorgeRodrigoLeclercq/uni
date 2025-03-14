#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/servicios_server.h"
#include "include/error.h"

struct Node *buscarNodo(List *tuplas, int key) {
    List aux = *tuplas; // Recorremos la lista
    while (aux != NULL) {
        if (aux->clave == key) {
            return aux;
        }
        aux = aux->next;
    }
    return NULL;  // No se ha encontrado
}

int insertarNodo(List *tuplas, struct Node *new_node) {
    if (*tuplas == NULL) {
        *tuplas = new_node;
    } else {
        new_node->next = *tuplas;
        *tuplas = new_node;
    }
    return 0;
}

int init_server(List *tuplas) {
    if (*tuplas == NULL) return 0;  // Lista vacía

    struct Node *siguiente;
    // Liberamos la lista enlazada
    while ((*tuplas)->next != NULL) {
        siguiente = (*tuplas)->next;
        free(*tuplas);
        *tuplas = siguiente;
    }
    free(*tuplas);
    *tuplas = NULL;
    return 0;
}

int set_value_server(List *tuplas, int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    if (exist_server(tuplas, key) == 1) { // Clave ya existe
        return -1;
    }
    if (N_value2 < 1 || N_value2 > 32)  // Validación de rango
        return -1;

    // Creamos el nuevo nodo
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if (new_node == NULL) return -1;

    new_node->clave = key;
    strncpy(new_node->valor1, value1, sizeof(new_node->valor1));
    new_node->n = N_value2;
    memcpy(new_node->valor2, V_value2, N_value2 * sizeof(double));
    // NUEVO: Asignamos el campo value3
    new_node->valor3 = value3;

    return insertarNodo(tuplas, new_node);
}

int get_value_server(List *tuplas, int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3) {
    if (*tuplas == NULL) return -1;  // Lista vacía

    struct Node *found_node = buscarNodo(tuplas, key);
    if (found_node != NULL) {
        // Copiamos los datos: usamos 256 como tamaño fijo para value1
        strncpy(value1, found_node->valor1, 256);
        *N_value2 = found_node->n;
        memcpy(V_value2, found_node->valor2, found_node->n * sizeof(double));
        // NUEVO: Devolvemos la estructura Coord
        *value3 = found_node->valor3;
        return 0;
    }
    return -1;  // Nodo no encontrado
}

int modify_value_server(List *tuplas, int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    if (N_value2 < 1 || N_value2 > 32)
        return -1;

    // Buscamos el nodo a modificar
    struct Node *nodo_a_modificar = buscarNodo(tuplas, key);
    if (nodo_a_modificar != NULL) {
        // Limpiamos campos existentes
        bzero(nodo_a_modificar->valor1, sizeof(nodo_a_modificar->valor1));
        bzero(nodo_a_modificar->valor2, N_value2 * sizeof(double));

        strncpy(nodo_a_modificar->valor1, value1, sizeof(nodo_a_modificar->valor1));
        nodo_a_modificar->n = N_value2;
        memcpy(nodo_a_modificar->valor2, V_value2, N_value2 * sizeof(double));
        // NUEVO: Actualizamos el valor3
        nodo_a_modificar->valor3 = value3;
        return 0;
    }
    return -1;
}

int delete_key_server(List *tuplas, int key) {
    // Buscamos el nodo a eliminar
    struct Node *nodo_a_eliminar = buscarNodo(tuplas, key);
    if (nodo_a_eliminar == NULL)
        return -1;

    // Si es el único nodo de la lista
    if (*tuplas == nodo_a_eliminar && (*tuplas)->next == NULL) {
        free(*tuplas);
        *tuplas = NULL;
        return 0;
    }

    // Si es el primer nodo
    if (*tuplas == nodo_a_eliminar) {
        *tuplas = (*tuplas)->next;
        free(nodo_a_eliminar);
        return 0;
    }

    // Buscamos el nodo anterior
    struct Node *anterior = *tuplas;
    while (anterior->next != nodo_a_eliminar)
        anterior = anterior->next;

    anterior->next = nodo_a_eliminar->next;
    free(nodo_a_eliminar);
    return 0;
}

int exist_server(List *tuplas, int key) {
    if (buscarNodo(tuplas, key) != NULL)
        return 1;
    else
        return 0;
    return -1;  // Nunca se alcanza
}

// Funciones de depuración
int printList(List l) {
    struct Node *aux = l;
    while (aux != NULL) {
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
    for (int i = 0; i < node->n; i++) {
        printf("\t· %lf\n", node->valor2[i]);
    }
    // NUEVO: Imprimimos la estructura Coord
    printf("Coord: (%d, %d)\n", node->valor3.x, node->valor3.y);
}
