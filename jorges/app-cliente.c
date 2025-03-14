#include <stdlib.h>
#include <stdio.h>
#include "include/error.h"
#include "include/claves.h"  // Incluye la definición de struct Coord y la API

// v_: valid petitions tests
// nv_: non-valid petitions tests

// ----------------------------- valid petitions -----------------------------
int v_complete_1(){
    if(destroy()){
        printerror("destroy()");
        return -1;
    }
    
    char valor1_test1[256] = "hice un set1";
    double valor2_test1[32] = {1, 2, 3, 4, 5};
    if(set_value(1, valor1_test1, 5, valor2_test1, (struct Coord){1,1})){
        printerror("set_value()");
        return -1;
    }
    
    char valor1_test2[256] = "hice un set2";
    double valor2_test2[32] = {1, 2, 3, 4};
    if(set_value(2, valor1_test2, 4, valor2_test2, (struct Coord){2,2})){
        printerror("set_value()");
        return -1;
    }
    
    if(exist(2) != 1){
        printerror("exist()");
        return -1;
    }
    
    char valor1_get[256];
    int n_valor2_get;
    double valor2_get[32];
    struct Coord coord_get;
    if(get_value(1, valor1_get, &n_valor2_get, valor2_get, &coord_get)){
        printerror("get_value()");
        return -1;
    }
    
    char valor1_test3[256] = "hice un set3";
    double valor2_test3[32] = {1, 2, 3};
    if(set_value(3, valor1_test3, 3, valor2_test3, (struct Coord){3,3})){
        printerror("set_value()");
        return -1;
    }
    
    char valor1_test4[256] = "cambiololololololol";
    double valor2_test4[32] = {1, 2, 3};
    if(modify_value(3, valor1_test4, 3, valor2_test4, (struct Coord){33,33})){
        printerror("modify_value()");
        return -1;
    }
    
    if(exist(3) != 1){
        printerror("exist()");
        return -1;
    }
    
    char valor1_get2[256];
    int n_valor2_get2;
    double valor2_get2[32];
    struct Coord coord_get2;
    if(get_value(3, valor1_get2, &n_valor2_get2, valor2_get2, &coord_get2)){
        printerror("get_value()");
        return -1;
    }
    
    if(delete_key(3)){
        printerror("delete()");
        return -1;
    }
    
    return 0;
}

int v_complete_2(){ 
    char valor1_test1[256] = "hice un set1";
    double valor2_test1[32] = {1, 2, 3, 4, 5};
    if(set_value(4, valor1_test1, 5, valor2_test1, (struct Coord){4,4})){
        printerror("set_value()");
        return -1;
    }
    
    char valor1_test2[256] = "hice un set2";
    double valor2_test2[32] = {1, 2, 3, 4};
    if(set_value(5, valor1_test2, 4, valor2_test2, (struct Coord){5,5})){
        printerror("set_value()");
        return -1;
    }
    
    if(destroy()){
        printerror("destroy()");
        return -1;
    }
    
    char valor1_test3[256] = "hice un set3";
    double valor2_test3[32] = {1, 2, 3};
    if(set_value(6, valor1_test3, 3, valor2_test3, (struct Coord){6,6})){
        printerror("set_value()");
        return -1;
    }
    
    char valor1_test4[256] = "cambiololololololol";
    double valor2_test4[32] = {1, 2, 3};
    if(modify_value(6, valor1_test4, 3, valor2_test4, (struct Coord){66,66})){
        printerror("modify_value()");
        return -1;
    }
    
    if(exist(6) != 1){
        printerror("exist()");
        return -1;
    }
    
    char valor1_get[256];
    int n_valor2_get;
    double valor2_get[32];
    struct Coord coord_get;
    if(get_value(6, valor1_get, &n_valor2_get, valor2_get, &coord_get)){
        printerror("get_value()");
        return -1;
    }
    
    if(delete_key(6)){
        printerror("delete()");
        return -1;
    }
    
    return 0;
}

int v_set(){
    double valor2_test1[32] = {1, 2, 3, 4, 5};
    char valor1_test1[256] = "pine";
    if(set_value(7, valor1_test1, 5, valor2_test1, (struct Coord){7,7})){
        printerror("set_value()");
        return -1;
    }
    
    double valor2_test2[32] = {10, 20, 30, 40};
    char valor1_test2[256] = "";
    if(set_value(8, valor1_test2, 4, valor2_test2, (struct Coord){8,8})){
        printerror("set_value()");
        return -1;
    }
    
    double valor2_test3[32] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    char valor1_test3[256] = "maple";
    if(set_value(9, valor1_test3, 10, valor2_test3, (struct Coord){9,9})) {
        printerror("set_value()");
        return -1;
    }
    
    return 0;
}

int v_get(){
    double valor2_test2[32] = {10, 20, 30, 40};
    char valor1_test2[256] = "";
    if(set_value(10, valor1_test2, 4, valor2_test2, (struct Coord){10,10})){
        printerror("set_value()");
        return -1;
    }
    
    double valor2_test1[32];
    char valor1_test1[256];
    int n_test1;
    struct Coord coord_get;
    if(get_value(10, valor1_test1, &n_test1, valor2_test1, &coord_get)){
        printerror("get_value()");
        return -1;
    }
    
    return 0;
}

int v_modify(){ 
    char valor1_test[256] = "hice un set3";
    double valor2_test[32] = {1, 2, 3};
    if(set_value(11, valor1_test, 3, valor2_test, (struct Coord){11,11})){
        printerror("set_value()");
        return -1;
    }
    
    char valor1_t[256] = "cambiololololololol";
    double valor2_t[32] = {1, 2, 3};
    if(modify_value(11, valor1_t, 3, valor2_t, (struct Coord){111,111})){
        printerror("modify_value()");
        return -1;
    }
    
    return 0;
}

int v_delete(){
    double valor2_test1[32] = {1, 2, 3, 4, 5};
    char valor1_test1[256] = "pine";
    if(set_value(12, valor1_test1, 5, valor2_test1, (struct Coord){12,12})){
        printerror("set_value()");
        return -1;
    }
    
    if(delete_key(12)){
        printerror("delete()");
        return -1;
    }
    
    return 0;
}

int v_exist_true(){
    double valor2_test1[32] = {1, 2, 3, 4, 5};
    char valor1_test1[256] = "pine";
    if(set_value(13, valor1_test1, 5, valor2_test1, (struct Coord){13,13})){
        printerror("set_value()");
        return -1;
    }
    
    if(exist(13) == 0){
        printerror("exist()");
        return -1;
    }
    
    return 0;
}

int v_exist_false(){
    destroy();
    if(exist(1)){
        printerror("exist()");
        return -1;
    }
    return 0;
}

// --------------------------- non-valid petitions ---------------------------
int nv_complete1(){
    char valor1_test1[256] = "hice un set1";
    double valor2_test1[32] = {1, 2, 3, 4, 5};
    if(set_value(1, valor1_test1, 5, valor2_test1, (struct Coord){1,1})){
        printerror("set_value()");
        return -1;
    }
    
    char valor1_test[256] = "hice un set2";
    double valor2_test[32] = {1, 2, 3, 4};
    if(set_value(2, valor1_test, 4, valor2_test, (struct Coord){2,2})){
        printerror("set_value()");
        return -1;
    }
    
    if(destroy()){
        printerror("destroy()");
        return -1;
    }
    
    // La clave no debería existir tras destroy()
    if(exist(2) != 0){
        printerror("exist()");
        return -1;
    }
    
    char valor1_get[256];
    int n_valor2_get;
    double valor2_get[32];
    struct Coord dummy;
    if(get_value(1, valor1_get, &n_valor2_get, valor2_get, &dummy) == 0){
        printerror("get_value()");
        return -1;
    }
    
    return 0;
}

int nv_length_errors(){
    double valo2_test4[32] = {100, 200, 300, 400};
    char alo_test4[259] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    if(set_value(5, alo_test4, 4, valo2_test4, (struct Coord){5,5}) == 0)
        printerror("set_value()");
    
    double valo2_test2[33] = {10, 20};
    char alo_test2[256] = "";
    if(set_value(3, alo_test2, 33, valo2_test2, (struct Coord){3,3}) == 0)
        printerror("set_value()");
    
    return 0;
}

int main(){
    if(v_complete_1()) return -1;
    if(v_complete_2()) return -1;
    if(v_set()) return -1;
    if(v_get()) return -1;
    if(v_modify()) return -1;
    if(v_delete()) return -1;
    if(v_exist_true()) return -1;
    if(v_exist_false()) return -1;
    if(nv_complete1()) return -1;
    if(nv_length_errors()) return -1;
    return 0;
}
