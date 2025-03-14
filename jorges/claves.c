#include <stdio.h>
#include <string.h>

#include "include/claves.h"
#include "include/error.h"
#include "include/mensaje.h"

int destroy(void){
    struct peticion pc;     // Creamos mensaje petición

    // Inicializamos la estructura del mensaje
    bzero(&pc, sizeof(struct peticion));

    // Rellenamos campos
    pc.operacion = 1;

    // Enviamos y recibimos respuesta
    return envio_respuesta(&pc);
}

int set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    struct peticion pc;     // Creamos mensaje petición

    // Inicializamos la estructura del mensaje
    bzero(&pc, sizeof(struct peticion));

    // Rellenamos campos
    pc.operacion = 2;
    pc.n = N_value2;
    pc.clave = key;

    // Comprobamos que la longitud de los arrays es la correcta
    if (strlen(value1) > 256 || sizeof(V_value2) / sizeof(V_value2[0]) > 32) {   
        return -1;
    }

    strncpy(pc.valor1, value1, sizeof(pc.valor1));
    if (N_value2 > 32) {
        memcpy(pc.valor2, V_value2, 32 * sizeof(double)); 
    } else {
        memcpy(pc.valor2, V_value2, N_value2 * sizeof(double)); 
    }
    
    // NUEVO: Asignamos el valor de value3
    pc.valor3 = value3;

    // Enviamos y recibimos respuesta
    return envio_respuesta(&pc);
}

int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3){
    struct peticion pc;     // Creamos mensaje petición

    // Inicializamos la estructura del mensaje
    bzero(&pc, sizeof(struct peticion));

    // Rellenamos campos
    pc.operacion = 3;
    pc.clave = key;

    // Enviamos y recibimos respuesta
    int ret = envio_respuesta(&pc);

    // Copiamos los datos obtenidos de la respuesta
    strncpy(value1, pc.valor1, sizeof(pc.valor1));
    memcpy(V_value2, pc.valor2, pc.n * sizeof(double));
    *N_value2 = pc.n;
    *value3 = pc.valor3; // NUEVO: Copiamos la estructura Coord

    return ret;
}

int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    struct peticion pc;     // Creamos mensaje petición

    // Inicializamos la estructura del mensaje
    bzero(&pc, sizeof(struct peticion));

    // Rellenamos campos
    pc.operacion = 4;
    pc.n = N_value2;
    pc.clave = key;

    // Comprobamos que la longitud de los arrays es la correcta
    if (strlen(value1) > 256 || sizeof(V_value2) / sizeof(V_value2[0]) > 32)
        return -1;
    
    strncpy(pc.valor1, value1, sizeof(pc.valor1));
    if (N_value2 > 32) {
        memcpy(pc.valor2, V_value2, 32 * sizeof(double)); 
    } else {
        memcpy(pc.valor2, V_value2, N_value2 * sizeof(double)); 
    }
    
    // NUEVO: Asignamos el valor de value3
    pc.valor3 = value3;

    // Enviamos y recibimos respuesta
    return envio_respuesta(&pc);
}

int delete_key(int key){
    struct peticion pc;     // Creamos mensaje petición

    // Inicializamos la estructura del mensaje
    bzero(&pc, sizeof(struct peticion));

    // Rellenamos campos
    pc.operacion = 5;
    pc.clave = key;

    // Enviamos y recibimos respuesta
    return envio_respuesta(&pc);
}

int exist(int key){
    struct peticion pc;     // Creamos mensaje petición

    // Inicializamos la estructura del mensaje
    bzero(&pc, sizeof(struct peticion));

    // Rellenamos campos
    pc.clave = key;
    pc.operacion = 6;

    // Enviamos y recibimos respuesta
    return envio_respuesta(&pc);
}
