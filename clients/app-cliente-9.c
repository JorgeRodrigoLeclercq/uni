#include "../include/claves-proxy-mq.h"
#include "../include/error.h"

int test_invalid_complete(){
    char value1_test1[256] = "Hello, World! 1";
    double value2_test1[32] = {0, 1, 2, 3, 4};
    struct Coord value3_test1 = {0, 0};
    if (set_value(1, value1_test1, 5, value2_test1, value3_test1)) {
        print_error("set_value()");
        return -1;
    }

    char value1_test2[256] = "Hello, World! 2";
    double value2_test2[32] = {0, 1, 2};
    struct Coord value3_test2 = {1, 1};
    if(set_value(2, value1_test2, 3, value2_test2, value3_test2)) {
        print_error("set_value()");
        return -1;
    }

    if (destroy()) {    
        print_error("destroy()");
        return -1;
    }
 
    char value1_get[256];
    int N_value2_get;
    double value2_get[32];
    struct Coord value3_get;
    if(get_value(1, value1_get, &N_value2_get, value2_get, &value3_get) == 0) {
        print_error("get_value()");
        return -1;
    }

    if (exist(2)!=0)  {
        print_error("exists()");
        return -1;
    }

    return 0;
}

int main() {
    if(test_invalid_complete()) return -1;
    return 0;
}