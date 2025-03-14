#include "include/mensaje.h"
#include "include/claves_servidor.h"

#include <stdio.h>
#include <mqueue.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t m_req, m_tuples;
pthread_cond_t cond_req;
int req_flag = 0;      

List tuples;

int handle_request(struct request *r) {
    struct request req;
    struct response res;        
    mqd_t client_queue;                
    struct mq_attr attr_cliente;
    int err;        
    
    pthread_mutex_lock(&m_req);
    req = *r;
    req_flag = 1;
    pthread_cond_signal(&cond_req);
    pthread_mutex_unlock(&m_req);

    bzero(&res, sizeof(struct response)) ; 
    
    bzero(&attr_cliente, sizeof(struct mq_attr));
    attr_cliente.mq_flags   = 0;
    attr_cliente.mq_maxmsg  = 10;
    attr_cliente.mq_msgsize = sizeof(struct response);
    attr_cliente.mq_curmsgs = 0;

    switch (req.operation) {
        case 1: 
            pthread_mutex_lock(&m_tuples);
            res.status = init_server(&tuples);  
            pthread_mutex_unlock(&m_tuples);

            break;     

        case 2: 
            pthread_mutex_lock(&m_tuples);
            res.status = set_value_server(&tuples, req.key, req.value1, req.N_value2, req.value2, req.value3);
            pthread_mutex_unlock(&m_tuples);
            fflush(stdout);

            break; 

        case 3: 
            pthread_mutex_lock(&m_tuples);
            res.status = get_value_server(&tuples, req.key, req.value1, &req.N_value2, req.value2, req.value3); 
            pthread_mutex_unlock(&m_tuples);

            strcpy(res.value1, req.value1);

            res.N_value2 = req.N_value2;
            memcpy(res.value2, req.value2, sizeof(req.value2));

            res.value3 = req.value3;

            break;

        case 4: 
            pthread_mutex_lock(&m_tuples);
            res.status = modify_value_server(&tuples, req.key, req.value1, req.N_value2, req.value2, req.value3);
            pthread_mutex_unlock(&m_tuples);
            
            break;
        
        case 5: 
            pthread_mutex_lock(&m_tuples);
            res.status = delete_key_server(&tuples, req.key);
            pthread_mutex_unlock(&m_tuples);
            
            break;
        case 6: 
            pthread_mutex_lock(&m_tuples);
            res.status = exist_server(&tuples, req.key);
            pthread_mutex_unlock(&m_tuples);

            break;
    }
    
    client_queue = mq_open(req.client_queue, O_WRONLY | O_CREAT, 0400, &attr_cliente);
    if (client_queue < 0) {
        mq_close(client_queue);
        return -2;
    }

    if (mq_send(client_queue, (char *) &res, sizeof(struct response), 0)) return -2;
    if (mq_close(client_queue)) return -2;
    if (mq_unlink(req.client_queue)) return -2;

    pthread_exit(NULL);
};

int main () {
    struct request req;  
    mqd_t q_server;           
    struct mq_attr attr_server;  
    pthread_t thread;       
    pthread_attr_t attr;
    pthread_attr_init(&attr);    
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int err;                
    
    bzero(&attr_server, sizeof(struct mq_attr));
    attr_server.mq_flags   = 0;
    attr_server.mq_maxmsg  = 10;
    attr_server.mq_msgsize = sizeof(struct request);
    attr_server.mq_curmsgs = 0;
    
    if ((q_server = mq_open("/SERVER", O_CREAT | O_RDONLY, 0664, &attr_server)) < 0) return -2;
    if (pthread_mutex_init(&m_req, NULL)) return -2;
    if (pthread_mutex_init(&m_tuples, NULL)) return -2;
    if (pthread_cond_init(&cond_req, NULL)) return (pthread_mutex_destroy(&m_req), -2);

    while (1) {
        err = mq_receive(q_server, (char *) &req, sizeof(struct request), 0);
        if (err < 0) {
	        continue;
        }

        pthread_create(&thread, &attr, (void *) handle_request, (void *) &req);

        pthread_mutex_lock(&m_req);

        while (req_flag == 0){
            pthread_cond_wait(&cond_req, &m_req);
        }

        req_flag = 0;
        pthread_mutex_unlock(&m_req); 
    }

    return 0;
};