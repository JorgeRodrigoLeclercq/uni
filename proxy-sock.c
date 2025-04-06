#include "include/mensaje-proxy-sock.h" 

#include <pthread.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <endian.h>	

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
    req.operation = 3;

    send_request(&req);

    strncpy(value1, req.value1, sizeof(req.value1));

    *N_value2 = req.N_value2;
    memcpy(V_value2, req.value2, *N_value2 * sizeof(double));

    *value3 = req.value3;

    return 0;
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
    char response_buffer [1];
    int fd_server;
    struct sockaddr_in address;
    int err;

    char *ip_tuples = getenv("IP_TUPLAS");
    char *port_tuples = getenv("PORT_TUPLAS");

    fd_server = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_server <= 0) return -2;

    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(port_tuples));

    err = inet_pton(AF_INET, ip_tuples, &address.sin_addr);
    if (err <= 0) { 
        close(fd_server);
        return -2;
    } 

    err = connect(fd_server, (struct sockaddr *)&address, sizeof(address)) ;
     if (err < 0) { 
        close(fd_server) ;
        return -2;
    } 

    int operation = htonl(req->operation);

    err = send_message(fd_server, (char *)&operation, sizeof(int));
    if (err < 0) {
        close(fd_server);
        return -2;
    }
    
    int key = htonl(req->key);
    int N_value2 = htonl(req->N_value2);
    double value2[32];
    int value3_x = htonl(req->value3.x);
    int value3_y = htonl(req->value3.y);

    switch (req->operation) {
        case 1: 
            break;

        case 2:
            err = send_message(fd_server, (char *)&key, sizeof(int));
            if (err < 0) {
                close(fd_server);
                return -2;
            }
      
            err = send_message(fd_server, req->value1, strlen(req->value1)+1);
            if (err < 0) {
                close(fd_server);
                return -2;
            }

            err = send_message(fd_server, (char *)&N_value2, sizeof(int));
            if (err < 0) {
                close(fd_server);
                return -2;
            }

            for (int i = 0; i < req->N_value2; i++) {
                value2[i] = htobe64(req->value2[i]);
            }
      
			for (int i = 0; i < req->N_value2; i++) {
                err = send_message(fd_server, (char *)&value2[i], sizeof(double));
				if (err < 0) {
					close(fd_server);
					return -2;
				}
            }
            
            err = send_message(fd_server, (char *)&value3_x, sizeof(int));
            if (err < 0) {
                close(fd_server);
                return -2;
            }
            
            err = send_message(fd_server, (char *)&value3_y, sizeof(int));
            if (err < 0) {
                close(fd_server);
                return -2;
            }

            break;

        case 3: 
            err = send_message(fd_server, (char *)&key, sizeof(int));
            if (err < 0) {
                close(fd_server);
                return -2;
            }

            ssize_t bytes_leidos = read_line(fd_server, req->value1, 256);
            if (bytes_leidos <= 0) {
                close(fd_server);
                return -2;
            }
            
            int received_N_value2;
            err = receive_message(fd_server, (char *)&received_N_value2, sizeof(int));
            if (err < 0) {
                close(fd_server);
                return -2;
            }
            
            req->N_value2 = ntohl(received_N_value2);
            
            for (int i = 0; i < req->N_value2; i++)
            {
                err = receive_message(fd_server, (char *)&req->value2[i], sizeof(double));
                if (err < 0) {
                    close(fd_server);
                    return -2;
                }
                req->value2[i] = be64toh(req->value2[i]);
            }
     
            int received_value3_x;
            err = receive_message(fd_server, (char *)&received_value3_x, sizeof(int));
            if (err < 0) {
                close(fd_server);
                return -2;
            }
            
            int received_value3_y;
            err = receive_message(fd_server, (char *)&received_value3_y, sizeof(int));
            if (err < 0) {
                close(fd_server);
                return -2;
            }
            
            break;

        case 4: 
            err = send_message(fd_server, (char *)&key, sizeof(int));
            if (err < 0) {
                close(fd_server);
                return -2;
            }

            err = send_message(fd_server, req->value1, strlen(req->value1)+1);
            if (err < 0) {
                close(fd_server);
                return -2;
            }

            err = send_message(fd_server, (char *)&N_value2, sizeof(int));
            if (err < 0) {
                close(fd_server);
                return -2;
            }

            for (int i = 0; i < req->N_value2; i++) {
                value2[i] = htobe64(req->value2[i]);
            }

            for (int i = 0; i < req->N_value2; i++) {
                err = send_message(fd_server, (char *)&value2[i], sizeof(double));
				if (err < 0) {
					close(fd_server);
					return -2;
				}
            }

            err = send_message(fd_server, (char *)&value3_x, sizeof(int));
            if (err < 0) {
                close(fd_server);
                return -2;
            }

            err = send_message(fd_server, (char *)&value3_y, sizeof(int));
            if (err < 0) {
                close(fd_server);
                return -2;
            }
            
            break;
        
        case 5: 
            err = send_message(fd_server, (char *)&key, sizeof(int));
            if (err < 0) {
                close(fd_server);
                return -2;
            }

            break ;

        case 6: 
            err = send_message(fd_server, (char *)&key, sizeof(int));
            if (err < 0) {
                close(fd_server);
                return -2;
            }

            break;
    }

    int status;

    err = receive_message(fd_server, (char *)&status, sizeof(int));
    if (err < 0) {
        close(fd_server);
        return -2;
    }
    status = ntohl(status);

    err = close(fd_server);
    if (err < 0) return -2;
    
    return status;
}

int send_message(int socket, char* buffer, int len) {
    int r;
    int l = len;

    do {	
        r = write(socket, buffer, l);
        l = l -r;
        buffer = buffer + r;
    } while ((l>0) && (r>=0));
    
    if (r < 0) return -2;   
    
    return 0;
}

int receive_message(int socket, char *buffer, int len) {
    int r;
    int l = len;
        
    do {
        r = read(socket, buffer, l);
        l = l -r ;
                //
buffer = buffer + r;
    } while ((l>0) && (r>=0));
    
    if (r < 0) return -2;   
    
    return 0;	
}

size_t read_line(int fd, void *buffer, size_t N_value2) {
    ssize_t bytes_read;  
    size_t total_bytes_read;	
    char *buf;
    char ch;

    if (N_value2 <= 0 || buffer == NULL) { 
        errno = EINVAL;
        return -1; 
    }
    
    buf = buffer;
    total_bytes_read = 0;
    
    for (;;) {
        bytes_read = read(fd, &ch, 1);	

        if (bytes_read == -1) {	
            if (errno == EINTR)
                continue;

            return -1;	

        } else if (bytes_read == 0) {	
            if (total_bytes_read == 0) return 0;

            else break;

        } else {			
            if (ch == '\n' || ch == '\0') break;
    
            if (total_bytes_read < N_value2 - 1) {	
                ++total_bytes_read;
                *buf++ = ch; 
            }
        } 
    }

    *buf = '\0';
    return total_bytes_read;
}