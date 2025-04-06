#include "../include/claves-proxy-sock.h"
#include "../include/error.h"

int test_set() {
    char value1_set[256] = "";
    double value2_set[32] = {0, 0};
    struct Coord value3_set = {0, 0};
    if (set_value(9, value1_set, 2, value2_set, value3_set)) {
        print_error("set_value()");
        return -1;
    }
    
    return 0;
}

int main() {
    if(test_set()) return -1;
    return 0;
}