#include "../include/claves-proxy-mq.h"
#include "../include/error.h"

int test_exist_true(){
    char value1_set[256] = "Hello, World! 144444444444444444444444444444444444444444444";
    double value2_set[32] = {14, 14, 14, 14, 14, 14, 14, 14, 5, 4, 3, 2, 1, 0};
    struct Coord value3_set;
    if (set_value(14, value1_set, 14, value2_set, value3_set)){
        print_error("set_value()");
        return -1;
    }
    
    if(exist(14)==0){
         print_error("exist()");
        return -1;
    } 

    return 0;
}

int main() {
    if(test_exist_true()) return -1;
    return 0;
}