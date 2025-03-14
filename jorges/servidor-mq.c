#include <stdio.h>
#include <mqueue.h>
#include <pthread.h>
#include <string.h>

#include "include/mensaje.h"
#include "include/servicios_server.h"
#include "include/error.h"

// Mutex y variables para proteger la copia del mensaje en el hilo y la atomicidad de las funciones.
pthread_mutex_t m_msj, acceso_tuplas;
pthread_cond_t cond_msj;
int msj_copiado = 0;      // 1 si se ha copiado.

// Lista enlazada para almacenar las tuplas
List tuplas;

int tratarPeticion(struct peticion *msj) {
    struct peticion mensaje;        // Copia local de la petición
    mqd_t q_cliente;                // Cola del cliente
    struct mq_attr attr_cliente;
    struct respuesta respuesta;
    int err;  // Resultado de las llamadas al sistema

    // Sección crítica: copiamos el mensaje recibido
    pthread_mutex_lock(&m_msj);
    mensaje = *msj;
    msj_copiado = 1;
    pthread_cond_signal(&cond_msj);
    pthread_mutex_unlock(&m_msj);

    // Inicializamos la respuesta
    bzero(&respuesta, sizeof(struct respuesta));

    // Configuración de la cola del cliente
    bzero(&attr_cliente, sizeof(struct mq_attr));
    attr_cliente.mq_flags   = 0;
    attr_cliente.mq_maxmsg  = 10;
    attr_cliente.mq_msgsize = sizeof(struct respuesta);
    attr_cliente.mq_curmsgs = 0;

    // Ejecutamos la petición según la operación solicitada
    switch (mensaje.operacion) {
        case 1: // INIT
            pthread_mutex_lock(&acceso_tuplas);
            respuesta.estado = init_server(&tuplas);
            pthread_mutex_unlock(&acceso_tuplas);
            break;

        case 2: // SET
            pthread_mutex_lock(&acceso_tuplas);
            respuesta.estado = set_value_server(&tuplas, mensaje.clave, mensaje.valor1, mensaje.n, mensaje.valor2, mensaje.valor3);
            pthread_mutex_unlock(&acceso_tuplas);
            fflush(stdout);
            break;

        case 3: // GET
            pthread_mutex_lock(&acceso_tuplas);
            respuesta.estado = get_value_server(&tuplas, mensaje.clave, mensaje.valor1, &mensaje.n, mensaje.valor2, &mensaje.valor3);
            pthread_mutex_unlock(&acceso_tuplas);
            respuesta.n = mensaje.n;
            strcpy(respuesta.valor1, mensaje.valor1);
            memcpy(respuesta.valor2, mensaje.valor2, sizeof(mensaje.valor2));
            // NUEVO: Se copia el campo value3
            respuesta.valor3 = mensaje.valor3;
            break;

        case 4: // MODIFY
            pthread_mutex_lock(&acceso_tuplas);
            respuesta.estado = modify_value_server(&tuplas, mensaje.clave, mensaje.valor1, mensaje.n, mensaje.valor2, mensaje.valor3);
            pthread_mutex_unlock(&acceso_tuplas);
            break;

        case 5: // DELETE
            pthread_mutex_lock(&acceso_tuplas);
            respuesta.estado = delete_key_server(&tuplas, mensaje.clave);
            pthread_mutex_unlock(&acceso_tuplas);
            break;

        case 6: // EXIST
            pthread_mutex_lock(&acceso_tuplas);
            respuesta.estado = exist_server(&tuplas, mensaje.clave);
            pthread_mutex_unlock(&acceso_tuplas);
            break;
    }

    // Abrimos la cola del cliente
    q_cliente = mq_open(mensaje.cola_cliente, O_WRONLY | O_CREAT, 0400, &attr_cliente);
    if (q_cliente < 0) {
        printerror("No se ha podido abrir la cola del cliente");
        mq_close(q_cliente);
        return -1;
    }

    // Enviamos la respuesta al cliente
    err = mq_send(q_cliente, (char *)&respuesta, sizeof(struct respuesta), 0);
    if (err < 0) {
        printerror("No se ha podido enviar la respuesta al cliente");
    }

    // Cerramos y eliminamos la cola del cliente
    if (mq_close(q_cliente)) {
        printerror("No se ha podido cerrar la cola del mensaje");
    }
    if (mq_unlink(mensaje.cola_cliente)) {
        printerror("No se ha podido borrar la cola del mensaje");
    }

    pthread_exit(NULL);
}

int main() {
    mqd_t q_server;         // Cola del servidor
    struct peticion msj;    // Mensaje recibido
    struct mq_attr attr_server;  // Atributos de la cola del servidor
    int err;                // Resultado de las llamadas al sistema
    pthread_t thread;       // Hilo para atender al cliente
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // Configuración de la cola del servidor
    bzero(&attr_server, sizeof(struct mq_attr));
    attr_server.mq_flags   = 0;
    attr_server.mq_maxmsg  = 10;
    attr_server.mq_msgsize = sizeof(struct peticion);
    attr_server.mq_curmsgs = 0;

    // Abrimos la cola del servidor
    q_server = mq_open("/SERVIDOR", O_CREAT | O_RDONLY, 0664, &attr_server);
    if (q_server < 0) {
        printerror("No se puede crear la cola del servidor");
        return -1;
    }

    // Inicialización de mutex y condición
    if (pthread_mutex_init(&m_msj, NULL) != 0) {
        printerror("Error al inicializar el mutex");
        return -1;
    }
    if (pthread_mutex_init(&acceso_tuplas, NULL) != 0) {
        printerror("Error al inicializar el mutex");
        return -1;
    }
    if (pthread_cond_init(&cond_msj, NULL) != 0) {
        printerror("Error al inicializar la variable de condición");
        pthread_mutex_destroy(&m_msj);
        return -1;
    }

    // Bucle para recibir peticiones de clientes
    while (1) {
        err = mq_receive(q_server, (char *)&msj, sizeof(struct peticion), 0);
        if (err < 0) {
            printerror("mq_receive(peticion-cliente)");
            continue;
        }

        pthread_create(&thread, &attr, (void *)tratarPeticion, (void *)&msj);

        pthread_mutex_lock(&m_msj);
        // Esperamos a que se copie el mensaje en el hilo antes de continuar
        while (msj_copiado == 0) {
            pthread_cond_wait(&cond_msj, &m_msj);
        }
        msj_copiado = 0;
        pthread_mutex_unlock(&m_msj);
    }
    return 0;
}
