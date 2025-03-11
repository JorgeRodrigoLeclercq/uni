#ifndef MENSAJE_H
#define MENSAJE_H

struct peticion {
    int clave;
    char valor1[256]; 
    int n;
    double valor2[32];
    int operacion;
    char cola_cliente[1024];    // Nombre de la cola del cliente
};

struct respuesta {
    int estado;
    char valor1[256];
    int n;
    double valor2[32];
};

int envio_respuesta (struct peticion *mensaje);

#endif // MENSAJE_H