//
// Created by golden on 11/12/24.
//

#ifndef PIXEL_STRUCTURES_HPP
#define PIXEL_STRUCTURES_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <tuple>

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
constexpr int hash_green_shift = 8;
constexpr int hash_blue_shift = 16;
template <>
struct std::hash<Pixel> {
  size_t operator()(Pixel const & pix) const noexcept {
    return hash<int>()(pix.channels.red) bitor (hash<int>()(pix.channels.green) << hash_green_shift) bitor
           (hash<int>()(pix.channels.blue) << hash_blue_shift);
  }
};

// Structure of Arrays (SoA)
struct SoA {
    std::vector<uint16_t> r, g, b;  // arrays distintos para el rojo, el verde y el azul
};

#endif //PIXEL_STRUCTURES_HPP
