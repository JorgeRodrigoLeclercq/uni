#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <cstdint>
#include <map>
#include <variant>
#include <gsl/span>

const int DEFAULT_MAX_COLOR = 255; // Constante para el valor por defecto de max_color

// Estructura para representar los canales de color de un píxel
struct ColorChannels {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
};

// Definición de Pixel utilizando ColorChannels
struct Pixel {
    ColorChannels channels;

    Pixel(uint16_t red = 0, uint16_t green = 0, uint16_t blue = 0)
      : channels{red, green, blue} {}

    bool operator<(const Pixel &other) const {
      return std::tie(channels.red, channels.green, channels.blue) <
             std::tie(other.channels.red, other.channels.green, other.channels.blue);
    }
};

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

    ImageHeader(std::string magic = "", ImageDimensions dims = {0, 0}, int max_c = DEFAULT_MAX_COLOR)
      : magic_number(std::move(magic)), dimensions(dims), max_color(max_c) {}
};

// Funciones para manipular imágenes

void get_header(std::ifstream & infile, ImageHeader & header);

void get_pixels(std::ifstream &infile, std::vector<Pixel> &pixel_data, unsigned long long pixel_count, bool is_16_bit);

void write_info(std::ofstream &outfile, const ImageHeader &header, const std::vector<Pixel> &pixel_data, bool is_16_bit);

void write_cppm(std::ofstream &cppm_outfile, const ImageHeader &header, const std::vector<Pixel> &pixel_data);

void maxlevel(int new_maxlevel, bool& is_16_bit, gsl::span<Pixel> &pixel_data, ImageHeader &header);

double interpolacion(const std::vector<double> &first_point, const std::vector<double> &second_point, int y_value);

Pixel interpolacion_colores(const std::vector<Pixel> &pixel_data, const std::vector<double> &coordinates, int width_count, const ImageHeader &original_header);

void DimensionChange(const ImageHeader &original_header, std::vector<Pixel> &pixel_data, ImageHeader &new_header);

#endif // FUNCTIONS_HPP





