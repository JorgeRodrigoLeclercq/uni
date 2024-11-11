//
// Created by golden on 11/8/24.
//

#ifndef CUTFREQ_HPP
#define CUTFREQ_HPP
#include "common/priority_queue.hpp"
#include "functions.hpp"

#include <unordered_map>
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

std::unordered_map<Pixel, int> contarFrecuencias(const SoA& pixel_data);
Bounded_priority_queue<Pixel, int> menosFrecuentes(const std::unordered_map<Pixel,int>& colores, int size);
double calcularDistancia(const Pixel &pixel1, const Pixel &pixel2);
void cutfreq(SoA &pixel_data, int n_colors);
#endif //CUTFREQ_HPP
