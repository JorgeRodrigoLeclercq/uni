#include "../include/mensaje-proxy-mq.h"
#include "../include/error.h"

int test_complete_1() {
    if (destroy()) {    
        print_error("destroy()");
        return -1;
    }

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
    if (exist(2) != 1) {
        print_error("exist()");
        return -1;
    }

    char value1_get[256];
    int N_value2_get;
    double value2_get[32];
    struct Coord value3_get;
    if(get_value(1, value1_get, &N_value2_get, value2_get, &value3_get)) {
        print_error("get_value()");
        return -1;
    }

    char value1_test3[256] = "Hello, World! 3";
    double value2_test3[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    struct Coord value3_test3 = {2, 2};
    if(set_value(3, value1_test3, 10, value2_test3, value3_test3)) {
        print_error("set_value()");
        return -1;
    }

    char value1_test4[256] = "Hello, World! 4";
    double value2_test4[32] = {0};
    struct Coord value3_test4 = {4, 8};
    if(modify_value(3, value1_test4, 1, value2_test4, value3_test4)) {
        print_error("modify_value()");
        return -1;
    }

    char value1_get2[256];
    int N_value2_get2;
    double value2_get2[32];
    struct Coord value3_get2;
    if(get_value(3, value1_get2, &N_value2_get2, value2_get2, &value3_get2)) {
        print_error("get_value()");
        return -1;
    }
    if(delete_key(3)) {
        print_error("delete()");
        return -1;
    }
    if (exist(3) == 1) {
        print_error("exists()");
        return -1;
    }

    return 0;
}

int main() {
    if(test_complete_1()) return -1;
    return 0;
}
