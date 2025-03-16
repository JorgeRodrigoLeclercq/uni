#include "../include/claves-proxy-mq.h"
#include "../include/error.h"

int test_invalid_length(){
    char long_sentence[256] = "The quick brown fox jumps over the lazy dog, while a curious cat watches from afar, pondering the mysteries of the universe. Meanwhile, the wind whispers through the trees, carrying the scent of rain, as distant thunder rumbles, signaling an approaching storm over the horizon.";
    double value2[32] = {3, 2, 1, 0};
    struct Coord value30 = {0, 0};
    if (set_value(0, long_sentence, 4, value2, value30) == 0) {
        print_error("set_value()");
        return -1;
    }

    char value1[256] = "";
    double long_vector[33] = {0};
    struct Coord value33 = {0, 0};
    if (set_value(3, value1, 33, long_vector, value33) == 0) {
        print_error("set_value()");
        return -1;
    }

    return 0;
}

int main() {
    if(test_invalid_length()) return -1;
    return 0;
}