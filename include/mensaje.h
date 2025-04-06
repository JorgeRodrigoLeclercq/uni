#ifndef MENSAJE_H
#define MENSAJE_H

#include "claves.h"

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

int send_message(int socket, char *buffer, int len);

int receive_message(int socket, char *buffer, int len);

ssize_t read_line(int fd, void *buffer, size_t n);

#endif // MENSAJE_H