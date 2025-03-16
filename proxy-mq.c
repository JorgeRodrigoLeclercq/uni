#include "include/mensaje-proxy-mq.h" 

#include <mqueue.h> 
#include <pthread.h> 
#include <stdio.h>
#include <string.h>
#include <strings.h>

int destroy() {
    struct request req;     

    bzero(&req, sizeof(struct request));

    req.operation = 1;

    return send_request(&req);
}

int set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    if (strlen(value1) > 255 || N_value2 > 32) return -1;

    struct request req;     

    bzero(&req, sizeof(struct request));

    req.key = key;

    strncpy(req.value1, value1, sizeof(req.value1));
    
    req.N_value2 = N_value2;
    memcpy(req.value2, V_value2, N_value2 * sizeof(double)); 

    req.value3 = value3;

    req.operation = 2;

    return send_request(&req);
}

int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3) {
    struct request req;     

    bzero(&req, sizeof(struct request));

    req.key = key;

    strncpy(value1, req.value1, sizeof(req.value1));

    *N_value2 = req.N_value2;
    memcpy(V_value2, req.value2, *N_value2 * sizeof(double));

    *value3 = req.value3;

    req.operation = 3;

    return send_request(&req);
}

int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    if (strlen(value1) > 255 || N_value2 > 32) return -1;

    struct request req;     

    bzero(&req, sizeof(struct request));

    req.key = key;

    strncpy(req.value1, value1, sizeof(req.value1));
    
    req.N_value2 = N_value2;
    memcpy(req.value2, V_value2, N_value2 * sizeof(double)); 

    req.value3 = value3;

    req.operation = 4;

    return send_request(&req);
}

int delete_key(int key) {
    struct request req;     

    bzero(&req, sizeof(struct request));

    req.key = key;

    req.operation = 5;

    return send_request(&req);
}

int exist(int key) {
    struct request req;     

    bzero(&req, sizeof(struct request));

    req.key = key;

    req.operation = 6;

    return send_request(&req);
}

int send_request(struct request *req) {
    char qc_name[1024];
    mqd_t q_server, q_client;     
    struct mq_attr attr_server, attr_client;
    struct response res; 
    int err;

    bzero(&attr_server, sizeof(struct mq_attr));
    attr_server.mq_flags   = 0;
    attr_server.mq_maxmsg  = 10;
    attr_server.mq_msgsize = sizeof(struct request);
    attr_server.mq_curmsgs = 0;

    bzero(&attr_client, sizeof(struct mq_attr));
    attr_client.mq_flags   = 0;
    attr_client.mq_maxmsg  = 10;
    attr_client.mq_msgsize = sizeof(struct response);
    attr_client.mq_curmsgs = 0;

    q_server = mq_open("/SERVER", O_WRONLY, 0700, &attr_server);
    if (q_server < 0) return -2;

    sprintf(qc_name, "%s%d", "/CLIENT", getpid());
    q_client = mq_open(qc_name, O_CREAT | O_RDONLY, 0664, &attr_client);
    if (q_client < 0) {
        if (mq_close(q_server)) return -2;
        
        return -2;
    }

    strncpy(req->client_queue, qc_name, sizeof(qc_name));
    err = mq_send(q_server, (char *)req, sizeof(struct request), 0) ;
    if (err < 0) {
        if (mq_close(q_server)) return -2;
        if (mq_close(q_client)) return -2;
        if (mq_unlink(qc_name)) return -2;

        return -2;
    }

    err = mq_receive(q_client, &res, sizeof(struct response), 0);
    if (err < 0) {
        if (mq_close(q_server)) return -2;
        if (mq_close(q_client)) return -2;
        if (mq_unlink(qc_name)) return -2;

        return -2;
    }

    if (req->operation == 3) {   
        strncpy(req->value1, res.value1, sizeof(res.value1));

        req->N_value2 = res.N_value2;
        memcpy(req->value2, res.value2, sizeof(res.value2)); 

        req->value3 = res.value3;
    }
    if (mq_close(q_server)) return -2;
    if (mq_close(q_client)) return -2;
    
    return res.status;
}