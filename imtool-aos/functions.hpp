#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "common/pixel_structures.hpp"

#include <cstdint>
#include <gsl/gsl>
#include <map>
#include <string>
#include <utility>
#include <vector>

constexpr int DEFAULT_MAX_COLOR = 255; // Constante para el valor por defecto de max_color

// Estructura para agrupar dimensiones de la imagen
struct ImageDimensions {
    int width;
    int height;
};

// Definición de ImageHeader utilizando ImageDimensions
struct ImageHeader {
    std::string magic_number;
    ImageDimensions dimensions;
    int max_color;

    explicit ImageHeader(std::string  magic = "", ImageDimensions const dims = {.width=0, .height = 0},
                         int const max_c = DEFAULT_MAX_COLOR)
      : magic_number(std::move(magic)), dimensions(dims), max_color(max_c) {}
};

// Funciones para manipular imágenes
void get_header(std::ifstream & infile, ImageHeader & header);

void get_pixels(std::ifstream &infile, std::vector<Pixel> &pixel_data, unsigned long long pixel_count, bool is_16_bit);

void write_info(std::ofstream &outfile, const ImageHeader &header, const std::vector<Pixel> &pixel_data, bool is_16_bit);

void write_color_table(std::ofstream &outfile, const std::vector<Pixel> &pixel_data, const std::map<Pixel, int> &color_table);

void write_cppm(std::ofstream &cppm_outfile, const ImageHeader &header, const std::vector<Pixel> &pixel_data);

#endif // FUNCTIONS_HPP





