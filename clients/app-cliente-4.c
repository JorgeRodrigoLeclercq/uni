#include "../include/claves-proxy-sock.h"
#include "../include/error.h"

int test_get() {
    char value1_set[256] = "";
    double value2_set[32] = {0, 0};
    struct Coord value3_set = {0, 0};
    if (set_value(10, value1_set, 2, value2_set, value3_set)) {
        print_error("set_value()");
        return -1;
    }

    char value1_get[256];
    int N_value2_get;
    double value2_get[32];
    struct Coord value3_get;
    if (get_value(10, value1_get, &N_value2_get, value2_get, &value3_get)){
        print_error("get_value()");
        return -1;
    }
    return 0;
}

int main() {
    if(test_get()) return -1;
    return 0;
}