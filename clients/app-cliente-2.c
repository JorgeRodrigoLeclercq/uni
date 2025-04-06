#include "../include/claves-proxy-sock.h"
#include "../include/error.h"

int test_complete_2() { 
    char value1_test5[256] = "Hello, World! 5";
    double value2_test5[32] = {0, 1, 2, 3, 4};
    struct Coord value3_test5 = {0, 0};
    if (set_value(5, value1_test5, 5, value2_test5, value3_test5)) {
        print_error("set_value()");
        return -1;
    }

    char value1_test6[256] = "Hello, World! 6";
    double value2_test6[32] = {0, 1, 2};
    struct Coord value3_test6 = {1, 1};
    if(set_value(6, value1_test6, 3, value2_test6, value3_test6)) {
        print_error("set_value()");
        return -1;
    }
    if (exist(2) != 1) {
        print_error("exist()");
        return -1;
    }

    char value1_get[256];
    int N_value2_get;
    double value2_get[32];
    struct Coord value3_get;
    if(get_value(5, value1_get, &N_value2_get, value2_get, &value3_get)) {
        print_error("get_value()");
        return -1;
    }

    char value1_test7[256] = "Hello, World! 7";
    double value2_test7[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct Coord value3_test7 = {2, 2};
    if(set_value(7, value1_test7, 10, value2_test7, value3_test7)) {
        print_error("set_value()");
        return -1;
    }

    char value1_test8[256] = "Hello, World! 8";
    double value2_test8[32] = {0};
    struct Coord value3_test8 = {5, 8};
    if(modify_value(7, value1_test8, 1, value2_test8, value3_test8)) {
        print_error("modify_value()");
        return -1;
    }

    char value1_get2[256];
    int N_value2_get2;
    double value2_get2[32];
    struct Coord value3_get2;
    if(get_value(7, value1_get2, &N_value2_get2, value2_get2, &value3_get2)) {
        print_error("get_value()");
        return -1;
    }
    if(delete_key(7)) {
        print_error("delete()");
        return -1;
    }
    if (exist(7) == 1) {
        print_error("exists()");
        return -1;
    }

    return 0;
}

int main() {
    if(test_complete_2()) return -1;
    return 0;
}