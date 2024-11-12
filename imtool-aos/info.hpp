#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "../common/header.hpp"
#include "../common/pixel_structures.hpp"

#include <gsl/gsl>
#include <vector>

// Funciones para manipular imágenes

void get_pixels(std::ifstream &infile, std::vector<Pixel> &pixel_data, unsigned long long pixel_count, bool is_16_bit);

void write_info(std::ofstream &outfile, const ImageHeader &header, const std::vector<Pixel> &pixel_data, bool is_16_bit);

#endif // FUNCTIONS_HPP





