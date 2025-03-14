#ifndef MENSAJE_H
#define MENSAJE_H

struct request {
    int key;
    char value1[256]; 
    int N_value2;
    double value2[32];
    struct Coord value3;
    int operation;
    char client_queue[1024];    
};

struct response {
    char value1[256];
    int N_value2;
    double value2[32];
    struct Coord value3;
    int status;
};

int send_request(struct request* req);

#endif // MENSAJE_H