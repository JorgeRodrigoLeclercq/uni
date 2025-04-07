#include "../include/claves-proxy-sock.h"
#include "../include/error.h"

int test_delete(){
    char value1_delete[256] = "Hello, World! 13";
    double value2_delete[32] = {13};
    struct Coord value3_delete = {13, 13};
    if (set_value(13, value1_delete, 1, value2_delete, value3_delete)) {
        print_error("set_value()");
        return -1;
    }
    if(delete_key(13)) {
        print_error("delete()");
        return -1;
    } 

    return 0;
}

int main() {
    if(test_delete()) return -1;
    return 0;
}