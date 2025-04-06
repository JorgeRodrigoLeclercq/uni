#include "include/mensaje.h"

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <endian.h>	
#include <errno.h>

pthread_mutex_t m_req, m_tuples;
pthread_cond_t cond_req;
int req_flag = 0;   

int handle_request(int *socket) {
    int fd;        
    int err;        
    
    pthread_mutex_lock(&m_req);
    fd = (*(int *)socket);
    req_flag = 1;
    pthread_cond_signal(&cond_req);
    pthread_mutex_unlock(&m_req);

    struct request req;
    struct response res;

    err = receive_message(fd, (char *)&req.operation, sizeof(int));
    if (err < 0) {
        close(fd);
        return -2;
    }

    req.operation = ntohl(req.operation);
    double value2[32];

    switch (req.operation) {
        case 1: 
            pthread_mutex_lock(&m_tuples);
            res.status = init();  
            pthread_mutex_unlock(&m_tuples);
            break;     

        case 2: 
            err = receive_message(fd, (char *)&req.key, sizeof(int));
            if (err < 0) {
                close(fd);
                return -2;
            }
            req.key = ntohl(req.key); 
            
            ssize_t total_bytes_read = read_line(fd, req.value1, 256);
            if (total_bytes_read <= 0) {
                close(fd);
                return -2;
            }

            err = receive_message(fd, (char *)&req.N_value2, sizeof(int));
            if (err < 0) {
                close(fd);
                return -2;
            }
            req.N_value2 = ntohl(req.N_value2); 
            
            
            for (int i = 0; i < req.N_value2; i++) {
                err = receive_message(fd, (char *)&req.value2[i], sizeof(double));
                if (err < 0) {
                    close(fd);
                    return -2;
                }
                req.value2[i] = be64toh(req.value2[i]);
            }

            err = receive_message(fd, (char *)&req.value3.x, sizeof(int));
            if (err < 0) {
                close(fd);
                return -2;
            }
            req.value3.x = ntohl(req.value3.x); 

            err = receive_message(fd, (char *)&req.value3.y, sizeof(int));
            if (err < 0) {
                close(fd);
                return -2;
            }
            req.value3.y = ntohl(req.value3.y);

            pthread_mutex_lock(&m_tuples);
            res.status = set_value(req.key, req.value1, req.N_value2, req.value2, req.value3);
            pthread_mutex_unlock(&m_tuples);

            break; 
        
        case 3: 
            err = receive_message(fd, (char *)&req.key, sizeof(int));
            if (err < 0) {
                close(fd);
                return -2;
            }
            req.key = ntohl(req.key); 

            pthread_mutex_lock(&m_tuples);
            res.status = get_value(req.key, req.value1, &req.N_value2, req.value2, &req.value3); 
            pthread_mutex_unlock(&m_tuples);
            
			err = send_message(fd, req.value1, strlen(req.value1)+1);
			if (err < 0) {
                close(fd);
				return -2;
			}
            
            int N_value2 = htonl(req.N_value2); 
            err = send_message(fd, (char *)&N_value2, sizeof(int));
            if (err < 0) {
                close(fd);
                return -2;
            }

            for (int i = 0; i < req.N_value2; ++i) {
                value2[i] = htobe64(req.value2[i]);
            }
            
            for (int i = 0; i < req.N_value2; i++) {
                err = send_message(fd, (char *)&value2[i], sizeof(double));
                if (err < 0) {
                    close(fd);
                    return -2;
                }
            } 
            
            req.value3.x = ntohl(req.value3.x);
            err = send_message(fd, (char *)req.value3.x, sizeof(int));
			if (err < 0) {
                close(fd);
				return -2;
			}

            req.value3.y = ntohl(req.value3.y);
            err = send_message(fd, (char *)req.value3.y, sizeof(int));
			if (err < 0) {
                close(fd);
				return -2;
			}

            break;

        case 4: 
            err = receive_message(fd, (char *)&req.key, sizeof(int));
            if (err < 0) {
                close(fd);
                return -2;
            }
            req.key = ntohl(req.key); 
            
            total_bytes_read = read_line(fd, req.value1, 256);
            if (total_bytes_read <= 0) {
                close(fd);
                return -2;
            }

            err = receive_message(fd, (char *)&req.N_value2, sizeof(int));
            if (err < 0) {
                close(fd);
                return -2;
            }
            req.N_value2 = ntohl(req.N_value2); 
            
            for (int i = 0; i < req.N_value2; i++) {
                err = receive_message(fd, (char *)&req.value2[i], sizeof(double));
                if (err < 0) {
                    close(fd);
                    return -2;
                }
                req.value2[i] = be64toh(req.value2[i]);
            }

            err = receive_message(fd, (char *)&req.value3.x, sizeof(int));
            if (err < 0) {
                close(fd);
                return -2;
            }
            req.value3.x = ntohl(req.value3.x); 

            err = receive_message(fd, (char *)&req.value3.y, sizeof(int));
            if (err < 0) {
                close(fd);
                return -2;
            }
            req.value3.y = ntohl(req.value3.y); 

            pthread_mutex_lock(&m_tuples);
            res.status = modify_value(req.key, req.value1, req.N_value2, req.value2, req.value3);
            pthread_mutex_unlock(&m_tuples);

            break;
        
        case 5: 
            
            err = receive_message(fd, (char *)&req.key, sizeof(int));
            if (err < 0) {
                close(fd);
                return -2;
            }
            req.key = ntohl(req.key); 

            pthread_mutex_lock(&m_tuples);
            res.status = delete_key(req.key);
            pthread_mutex_unlock(&m_tuples);
            break ;

        case 6: 
            
            err = receive_message(fd, (char *)&req.key, sizeof(int));
            if (err < 0) {
                close(fd);
                return -2;
            }
            req.key = ntohl(req.key);

            pthread_mutex_lock(&m_tuples);
            res.status = exist(req.key);
            pthread_mutex_unlock(&m_tuples);
            break ;
    }
    
    int status = htonl(res.status);

    err = send_message(fd, (char *)&status, sizeof(int));
    if (err < 0) {
        close(fd);
        return -2;
    }

    err = close(fd);
    if (err < 0) -2;

    pthread_exit(NULL);
};

int main (int argc, char *argv[]) {
    pthread_t thread;       
    pthread_attr_t attr;
    pthread_attr_init(&attr);    
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int err;   

    int fd_server, fd_client;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int val = 1;
    
    if (pthread_mutex_init(&m_req, NULL)) return -2;
    if (pthread_mutex_init(&m_tuples, NULL)) return -2;
    if (pthread_cond_init(&cond_req, NULL)) return (pthread_mutex_destroy(&m_req), -2);

    if ((fd_server = socket(AF_INET, SOCK_STREAM, 0)) == 0) -2;
    if (setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val))) return -2;

    bzero((char *)&address, sizeof(address));
    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons(atoi(argv[1])); 
     
    if (bind(fd_server, (struct sockaddr *)&address,  sizeof(address)) < 0) return -2;
    
    if (listen(fd_server, SOMAXCONN) < 0) return -2;

    while (1) {
        fd_client = accept(fd_server, (struct sockaddr *)&address,  (socklen_t*)&addrlen) ;
        if (fd_client <= 0) -2;

        pthread_create(&thread, &attr, (void *)handle_request, (int *)&fd_client);

        pthread_mutex_lock(&m_req);

        while (req_flag == 0){
            pthread_cond_wait(&cond_req, &m_req);
        }

        req_flag = 0;
        pthread_mutex_unlock(&m_req); 
    }

    return 0;
};

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
        buffer = buffer + r;
    } while ((l>0) && (r>=0));
    
    if (r < 0) return -2;   
    
    return(0);	
}

ssize_t read_line(int fd, void *buffer, size_t n)
{
    ssize_t bytes_read;  
    size_t total_bytes_read;	
    char *buf;
    char ch;

    if (n <= 0 || buffer == NULL) { 
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
            if (ch == '\n' || ch == '0') break;
    
            if (total_bytes_read < n - 1) {	
                ++total_bytes_read;
                *buf++ = ch; 
            }
        } 
    }

    *buf = '\0';
    return total_bytes_read;
}