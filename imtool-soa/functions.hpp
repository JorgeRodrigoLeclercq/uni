#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

// Structure of Arrays (SoA)
struct SoA {
    std::vector<uint8_t> r, g, b;  // arrays distintos para el rojo, el verde y el azul
};

void get_header(std::ifstream &infile, std::string &magic_number, int &width, int &height, int &max_color);
void get_pixels(std::ifstream &infile, SoA &pixel_data, int pixel_count, bool is_16_bit);

#endif // FUNCTIONS_HPP
