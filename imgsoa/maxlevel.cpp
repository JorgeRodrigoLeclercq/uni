//
// Created by fjavi on 11/11/2024.
//
#include "maxlevel.hpp"

#include "info.hpp"

#include <algorithm>  // Para std::clamp
#include <limits>

void maxlevel(int new_maxlevel, bool& is_16_bit, SoA& pixel_data, ImageHeader& header) {
  // Determinar si la salida será de 8 o 16 bits
  is_16_bit = new_maxlevel > MAX_COLOR_8BIT;

  // Escalar los componentes de color sin redondeo para cada canal
  for (size_t i = 0; i < pixel_data.r.size(); ++i) {
    uint16_t const r_scaled = static_cast<uint16_t>(
        std::clamp(static_cast<int>(static_cast<float>(pixel_data.r[i]) * static_cast<float>(new_maxlevel) / static_cast<float>(header.max_color)),
                   0, new_maxlevel));
    uint16_t const g_scaled = static_cast<uint16_t>(
        std::clamp(static_cast<int>(static_cast<float>(pixel_data.g[i]) * static_cast<float>(new_maxlevel) / static_cast<float>(header.max_color)),
                   0, new_maxlevel));
    uint16_t const b_scaled = static_cast<uint16_t>(
        std::clamp(static_cast<int>(static_cast<float>(pixel_data.b[i]) * static_cast<float>(new_maxlevel) / static_cast<float>(header.max_color)),
                   0, new_maxlevel));

    // Asignar los valores escalados
    pixel_data.r[i] = r_scaled;
    pixel_data.g[i] = g_scaled;
    pixel_data.b[i] = b_scaled;
  }

  // Actualizar max_color al nuevo nivel máximo
  header.max_color = new_maxlevel;
}