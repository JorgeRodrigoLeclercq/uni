#include "../include/claves-proxy-sock.h"
#include "../include/error.h"

int test_modify(){ 
    char value1_set[256] = "Hello, World! 11";
    double value2_set[32] = {1, 1, 1, 0, 1, 0, 0};
    struct Coord value3_set = {64, 64};
    if(set_value(11, value1_set, 7, value2_set, value3_set)) {
        print_error("set_value()");
        return -1;
    }

    char value1_modify[256] = "Hello, World! 12";
    double value2_modify[32] = {5, 5, 5, 5, 5};
    struct Coord value3_modify = {8, 8};
    if(modify_value(11, value1_modify, 1, value2_modify, value3_modify)) {
        print_error("modify_value()");
        return -1;
    }

    return 0;
}

int main() {
    if(test_modify()) return -1;
    return 0;
}