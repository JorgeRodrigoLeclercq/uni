#include <mqueue.h> 
#include <pthread.h> 
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "include/mensaje.h" 
#include "include/error.h"

int envio_respuesta (struct peticion *mensaje) {
    mqd_t q_server, q_cliente;      // colas para el servidor y cliente  
    int  err;
    char qc_name[1024] ;
    struct mq_attr attr_server, attr_cliente;
    struct respuesta m_respuesta; 

    // attr - cola server 
    bzero(&attr_server, sizeof(struct mq_attr));
    attr_server.mq_flags   = 0;
    attr_server.mq_maxmsg  = 10;
    attr_server.mq_msgsize = sizeof(struct peticion);
    attr_server.mq_curmsgs = 0;

    // attr - cola cliente
    bzero(&attr_cliente, sizeof(struct mq_attr));
    attr_cliente.mq_flags   = 0;
    attr_cliente.mq_maxmsg  = 10;
    attr_cliente.mq_msgsize = sizeof(struct respuesta);
    attr_cliente.mq_curmsgs = 0;

    // Abrimos la cola del servidor para escribir (enviar peticiones)
    q_server = mq_open("/SERVIDOR", O_WRONLY, 0700, &attr_server);
    if (q_server < 0) {
        printerror("Error al abrir la cola del servidor.");
        return -1;
    }

    // Abrimos cola cliente (read only) -> (escribirá el servidor)
    sprintf(qc_name, "%s%d", "/CLIENTE_", getpid()) ;
    q_cliente = mq_open(qc_name, O_CREAT | O_RDONLY, 0664, &attr_cliente);
    if (q_cliente < 0) {
        printerror("mq_open(cola_cliente)");
        mq_close(q_server);
        return -1;
    }

    //En la estructura de la petición copiamos el nombre de la cola del cliente (para que pueda acceder el server)
    strncpy(mensaje->cola_cliente, qc_name, sizeof(qc_name));

    // Enviamos la petición al servidor
    err = mq_send(q_server, (char *)mensaje, sizeof(struct peticion), 0) ;
    if (err < 0) {
        printerror("Error al enviar petición al server");
        if (mq_close(q_server)) {printerror("mq_close(q_server)");}
        if (mq_close(q_cliente)) {printerror("mq_close(q_cliente)");}
        if (mq_unlink(qc_name)) {printerror("mq_unlink(qc_name)");}
        return -1;
    }
    
    // Recibimos la respuesta del servidor
    err = mq_receive(q_cliente, &m_respuesta, sizeof(struct respuesta), 0);
    if (err < 0) {
        printerror("mq_receive(q_cliente) - respuesta server");
        if (mq_close(q_server)) {printerror("mq_close(q_server)");}
        if (mq_close(q_cliente)) {printerror("mq_close(q_cliente)");}
        if (mq_unlink("/SERVIDOR")) {printerror("mq_unlink(q_server)");}
        return -1;
    }
    
    // Copiamos en punteros si es get
    if (mensaje->operacion == 3)    // if operacion == GET
    {   
        mensaje->n = m_respuesta.n;
        strncpy(mensaje->valor1, m_respuesta.valor1, sizeof(m_respuesta.valor1));
        memcpy(mensaje->valor2, m_respuesta.valor2, sizeof(m_respuesta.valor2)); 
    }
    
    // Cerramos todas las colas
    if (mq_close(q_server)) {printerror("mq_close(q_server)");}
    
    if (mq_close(q_cliente)) {printerror("mq_close(q_cliente)");}
    
    return m_respuesta.estado;
}