//
// Created by fjavi on 11/11/2024.
//
#include "maxlevel.hpp"
#include <algorithm>
void maxlevel(int new_maxlevel, bool& is_16_bit, gsl::span<Pixel> &pixel_data, ImageHeader &header) {
  // Determinar si la salida será de 8 o 16 bits
  is_16_bit = new_maxlevel > MAX_COLOR_8BIT;

  for (auto& pixel : pixel_data) {
    // Escalar cada componente de color
    uint16_t r_scaled = static_cast<uint16_t>(
        std::clamp(static_cast<int>(static_cast<float>(pixel.channels.red) * static_cast<float>(new_maxlevel) / static_cast<float>(header.max_color)),
                   0, new_maxlevel));
    uint16_t g_scaled = static_cast<uint16_t>(
        std::clamp(static_cast<int>(static_cast<float>(pixel.channels.green) * static_cast<float>(new_maxlevel) / static_cast<float>(header.max_color)),
                   0, new_maxlevel));
    uint16_t b_scaled = static_cast<uint16_t>(
        std::clamp(static_cast<int>(static_cast<float>(pixel.channels.blue) * static_cast<float>(new_maxlevel) / static_cast<float>(header.max_color)),
                   0, new_maxlevel));

    // Asignar valores escalados en 8 o 16 bits
    pixel.channels.red = is_16_bit ? r_scaled : static_cast<uint8_t>(r_scaled);
    pixel.channels.green = is_16_bit ? g_scaled : static_cast<uint8_t>(g_scaled);
    pixel.channels.blue = is_16_bit ? b_scaled : static_cast<uint8_t>(b_scaled);
  }

  // Actualizar max_color al nuevo nivel máximo
  header.max_color = new_maxlevel;
}