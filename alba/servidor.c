#include <stdio.h>
#include <mqueue.h>
#include <pthread.h>
#include <string.h>

#include "include/mensaje.h"
#include "include/servicios_server.h"
#include "include/error.h"

// Mutex y variables para proteger la copia del msj en el hilo y la atomicidad de las funciones.
pthread_mutex_t m_msj, acceso_tuplas;
pthread_cond_t cond_msj;
int msj_copiado = 0;      // 1 si se ha copiado.

// Creamos la lista enlazada que guardará el sistema de tuplas
List tuplas;

int tratarPeticion (struct peticion *msj) {
    struct peticion mensaje;        // Mensaje local
    mqd_t q_cliente;                // Cola del cliente
    struct mq_attr attr_cliente;
    struct respuesta respuesta;
    int err;        // Almacena el resultado de las funciones de llamada al sistema
    
    // SECCION CRITICA
    // El thread copia el mensaje en un mensaje local
    pthread_mutex_lock(&m_msj);

    mensaje = *msj;

    // Ya puede despertar el servidor
    msj_copiado = 1;
    pthread_cond_signal(&cond_msj);

    pthread_mutex_unlock(&m_msj);

    bzero(&respuesta, sizeof(struct respuesta)) ; // Inicializamos la respuesta

    // attr - cola cliente
    bzero(&attr_cliente, sizeof(struct mq_attr));
    attr_cliente.mq_flags   = 0;
    attr_cliente.mq_maxmsg  = 10;
    attr_cliente.mq_msgsize = sizeof(struct respuesta);
    attr_cliente.mq_curmsgs = 0;

    // Ahora ejecutamos la peticion
    switch (mensaje.operacion) {
        case 1: // INIT
            pthread_mutex_lock(&acceso_tuplas);
            respuesta.estado = init_server(&tuplas);  // Metemos el return del init en el estado de la respuesta
            pthread_mutex_unlock(&acceso_tuplas);
            break ;     

        case 2: // SET
            pthread_mutex_lock(&acceso_tuplas);
            respuesta.estado = set_value_server(&tuplas, mensaje.clave, mensaje.valor1, mensaje.n, mensaje.valor2);
            pthread_mutex_unlock(&acceso_tuplas);
            // printList(tuplas);   //debug
            fflush(stdout);
            break ; 

        case 3: // GET
            pthread_mutex_lock(&acceso_tuplas);
            respuesta.estado = get_value_server(&tuplas, mensaje.clave, mensaje.valor1, &mensaje.n, mensaje.valor2); 
            pthread_mutex_unlock(&acceso_tuplas);
            respuesta.n = mensaje.n;
            strcpy(respuesta.valor1, mensaje.valor1);
            memcpy(respuesta.valor2, mensaje.valor2, sizeof(mensaje.valor2));
            break ;

        case 4: // MODIFY
            pthread_mutex_lock(&acceso_tuplas);
            respuesta.estado = modify_value_server(&tuplas, mensaje.clave, mensaje.valor1, mensaje.n, mensaje.valor2);
            pthread_mutex_unlock(&acceso_tuplas);
            // printList(tuplas);   //debug
            break ;
        
        case 5: // DELETE
            pthread_mutex_lock(&acceso_tuplas);
            respuesta.estado = delete_key_server(&tuplas, mensaje.clave);
            pthread_mutex_unlock(&acceso_tuplas);
            // printList(tuplas);   //debug
            break ;
        case 6: // EXIST
            pthread_mutex_lock(&acceso_tuplas);
            respuesta.estado = exist_server(&tuplas, mensaje.clave);
            pthread_mutex_unlock(&acceso_tuplas);
            break ;
    }

    // Abrimos la cola del cliente
    q_cliente = mq_open(mensaje.cola_cliente, O_WRONLY | O_CREAT, 0400, &attr_cliente);
    if (q_cliente < 0) {
        printerror("No se ha podido abrir la cola del cliente");
        mq_close(q_cliente);
        return -1;
    }

    err = mq_send(q_cliente, (char *) &respuesta, sizeof(struct respuesta), 0);
    if (err) {
        printerror("No se ha podido enviar la respuesta al cliente");
    }

    err = mq_close(q_cliente);
    if (err) {
        printerror("No se ha podido cerrar la cola del mensaje") ;
    }

    err = mq_unlink(mensaje.cola_cliente);
    if (err) {
        printerror("No se ha podido borrar la cola del mensaje") ;
    }

    pthread_exit(NULL);
};


int main () {
    mqd_t q_server;         // Cola del servidor 
    struct peticion msj;    // Mensaje que recibe 
    struct mq_attr attr_server;  // Atributos de la cola 
    int err;                // Almacena el resultado de las funciones de llamada al sistema
    pthread_t thread;       // Hilo que atiende a un cliente
    pthread_attr_t attr;
    pthread_attr_init(&attr);    // crea attr para el hilo
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // attr - cola server 
    bzero(&attr_server, sizeof(struct mq_attr));
    attr_server.mq_flags   = 0;
    attr_server.mq_maxmsg  = 10;
    attr_server.mq_msgsize = sizeof(struct peticion);
    attr_server.mq_curmsgs = 0;

    // Abrimos la cola del servidor
    q_server = mq_open("/SERVIDOR", O_CREAT|O_RDONLY, 0664, &attr_server);
    if (q_server < 0) {
        printerror("No se puede crear la cola del servidor");
        return -1 ;
    } 

    // Inicialización de los mutex
    if (pthread_mutex_init(&m_msj, NULL) != 0) {
        printerror("Error al inicializar el mutex");
        return -1;
    }

    if (pthread_mutex_init(&acceso_tuplas, NULL) != 0) {
        printerror("Error al inicializar el mutex");
        return -1;
    }

    // Inicialización de la variable de condición
    if (pthread_cond_init(&cond_msj, NULL) != 0) {
        printerror("Error al inicializar la variable de condición");
        pthread_mutex_destroy(&m_msj); // Liberar recursos del mutex
        return -1;
    }

    // Bucle para recibir las peticiones de los clientes
    while (1) {
        err = mq_receive(q_server, (char *) &msj, sizeof(struct peticion), 0);
        if (err < 0) {
	        printerror("mq_receive(peticion-cliente)");
	        continue;
        }

        pthread_create(&thread, &attr, (void *) tratarPeticion, (void *) &msj);

        pthread_mutex_lock(&m_msj);

        // Mientras no se haya copiado el msj del hilo anterior no desbloqueamos
        while (msj_copiado == 0){
            pthread_cond_wait(&cond_msj, &m_msj);
        }

        msj_copiado = 0;
        pthread_mutex_unlock(&m_msj); 

    }
    return 0;
};