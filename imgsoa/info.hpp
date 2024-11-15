#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "../common/header.hpp"
#include "../common/pixel_structures.hpp"

#include <gsl/span>

// Funciones para manipular imágenes

void get_pixels(std::ifstream &infile, SoA &pixel_data, unsigned long long pixel_count, bool is_16_bit);

void write_info(std::ofstream& outfile, ImageHeader const & header, SoA const & pixel_data, bool is_16_bit);

void info(const ImageHeader& header, const SoA& pixel_data);

#endif // FUNCTIONS_HPP
