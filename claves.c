#include "include/claves.h"
#include "include/mensaje.h"

#include <stdio.h>
#include <string.h>

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