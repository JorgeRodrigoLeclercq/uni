#include "../include/claves-proxy-mq.h"
#include "../include/error.h"

int test_exist_false(){
    destroy();
    
    if(exist(1)) {
        print_error("exist()");
        return -1;
    } 

    return 0;
}

int main() {
    if(test_exist_false()) return -1;
    return 0;
}