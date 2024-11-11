#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <cstdint>
#include <gsl/gsl>
#include <map>
#include <string>
#include <utility>
#include <vector>

constexpr int DEFAULT_MAX_COLOR = 255; // Constante para el valor por defecto de max_color

// Estructura para representar los canales de color de un píxel
struct ColorChannels {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
};

// Definición de Pixel utilizando ColorChannels
struct Pixel {
    ColorChannels channels;

    explicit Pixel(uint16_t const red = 0, uint16_t const green = 0, uint16_t const blue = 0)
      : channels{.red=red, .green=green, .blue=blue} {}

    bool operator<(const Pixel &other) const {
      return std::tie(channels.red, channels.green, channels.blue) <
             std::tie(other.channels.red, other.channels.green, other.channels.blue);
    }

    bool operator==(const Pixel &other) const {
        return channels.red == other.channels.red && channels.green == other.channels.green && channels.blue == other.channels.blue;
    }
};

// Especialización de std::hash para la clase Pixel
template <>
struct std::hash<Pixel> {
    size_t operator()(Pixel const & pix) const noexcept {
      return hash<int>()(pix.channels.red) ^ (hash<int>()(pix.channels.green) << 1) ^
             (hash<int>()(pix.channels.blue) << 2);
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

void maxlevel(int new_maxlevel, bool& is_16_bit, gsl::span<Pixel> &pixel_data, ImageHeader &header);

double interpolacion(const std::vector<double> &first_point, const std::vector<double> &second_point, int y_value);

Pixel interpolacion_colores(const std::vector<Pixel> &pixel_data, const std::vector<double> &coordinates, int width_count, const ImageDimensions &original_dimension);

void DimensionChange(ImageDimensions &original_dimension, std::vector<Pixel> &pixel_data, ImageHeader &new_header);

#endif // FUNCTIONS_HPP





