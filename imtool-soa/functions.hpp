#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "../common/header.hpp"

#include <cstdint>
#include <gsl/span>
#include <vector>

// Structure of Arrays (SoA)
struct SoA {
    std::vector<uint16_t> r, g, b;  // arrays distintos para el rojo, el verde y el azul
};

// Funciones para manipular imágenes

void get_pixels(std::ifstream &infile, SoA &pixel_data, unsigned long long pixel_count, bool is_16_bit);

void write_info(std::ofstream& outfile, ImageHeader& header, SoA& pixel_data, bool is_16_bit);

#endif // FUNCTIONS_HPP
