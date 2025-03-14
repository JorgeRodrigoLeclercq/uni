#include <stdlib.h>
#include <stdio.h>

// v_: valid petitions tests
// nv_: non-valid petitions tests

// ----------------------------- valid petitions -----------------------------
int v_complete_1() {
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

    if (exist(3) != 1) {
        print_error("exists()");
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
}

int v_complete_2() { 
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

    if (exist(7) != 1) {
        print_error("exists()");
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
}

int v_set() {
    char value1_set[256] = "";
    double value2_set = {0, 0};
    struct Coord value3_set = {0, 0};
    if (set_value(9, value1_set, 2, value2_set, value3_set)) {
        print_error("set_value()");
        return -1;
    }
    
    return 0;
}

int v_get() {
    char value1_set[256] = "";
    double value2_set = {0, 0};
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

int v_modify(){ 
    char value1_set[256] = "Hello, World! 11";
    double value2_set32] = {1, 1, 1, 0, 1, 0, 0};
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

int v_delete(){
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

int v_exist_true(){
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

int v_exist_false(){
    destroy();
    
    if(exist(1)) {
        print_error("exist()");
        return -1;
    } 

    return 0;
}

// --------------------------- non-valid petitions ---------------------------
int nv_complete1(){
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

int nv_length(){
    char long_sentence[256] = "The quick brown fox jumps over the lazy dog, while a curious cat watches from afar, pondering the mysteries of the universe. Meanwhile, the wind whispers through the trees, carrying the scent of rain, as distant thunder rumbles, signaling an approaching storm over the horizon.";
    double value2[32] = {3, 2, 1, 0};
    struct Coord value3 = {0, 0};
    if (set_value(0, long_sentence, 4, value2, value3) == 0) {
        print_error("set_value()");
        return -1;
    }

    char value1[256] = "";
    double long_vector[33] = {0};
    struct Coord value3 = {0, 0};
    if (set_value(3, value1, 33, long_vector, value3) == 0) {
        print_error("set_value()");
        return -1;
    }

    return 0;
}

int main() {
    if(v_complete_1()) return -1;
    if(v_complete_2()) return -1;
    if(v_set()) return -1;
    if(v_get()) return -1;
    if(v_modify()) return -1;
    if(v_delete()) return -1;
    if(v_exist_true()) return -1;
    if(v_exist_false()) return -1;
    if(nv_complete1()) return -1;
    if(nv_length()) return -1;
    return 0;
}
