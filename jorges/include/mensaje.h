#ifndef MENSAJE_H
#define MENSAJE_H

#include "claves.h"  // Asegura la definición de struct Coord

struct peticion {
    int clave;
    char valor1[256]; 
    int n;
    double valor2[32];
    int operacion;
    char cola_cliente[1024];    // Nombre de la cola del cliente
    struct Coord valor3;        // NUEVO: Almacena la estructura Coord (value3)
};

struct respuesta {
    int estado;
    char valor1[256];
    int n;
    double valor2[32];
    struct Coord valor3;        // NUEVO: Se devuelve también la estructura Coord
};

int envio_respuesta (struct peticion *mensaje);

#endif // MENSAJE_H
