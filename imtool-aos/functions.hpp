#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "../common/header.hpp"

#include <cstdint>
#include <gsl/gsl>
#include <map>
#include <string>
#include <vector>

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

// Funciones para manipular imágenes

void get_pixels(std::ifstream &infile, std::vector<Pixel> &pixel_data, unsigned long long pixel_count, bool is_16_bit);

void write_info(std::ofstream &outfile, const ImageHeader &header, const std::vector<Pixel> &pixel_data, bool is_16_bit);

#endif // FUNCTIONS_HPP





