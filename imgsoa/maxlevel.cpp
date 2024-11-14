//
// Created by javi on 11/11/2024.
//
#include "maxlevel.hpp"

#include "info.hpp"

#include <algorithm>  // Para std::clamp
#include <limits>

void maxlevel(int new_maxlevel, bool& is_16_bit, SoA& pixel_data, ImageHeader& header) {
  // Determinar si la salida será de 8 o 16 bits
  is_16_bit = new_maxlevel > MAX_COLOR_8BIT;

  // Obtener el factor de escala una sola vez
  const float scale_factor = static_cast<float>(new_maxlevel) / static_cast<float>(header.max_color);

  // Iterar una vez para los tres canales
  for (size_t i = 0; i < pixel_data.r.size(); ++i) {
    // Escalar los valores y asegurarse de que estén dentro del rango utilizando std::clamp
    const float r_scaled = static_cast<float>(pixel_data.r[i]) * scale_factor;
    const float g_scaled = static_cast<float>(pixel_data.g[i]) * scale_factor;
    const float b_scaled = static_cast<float>(pixel_data.b[i]) * scale_factor;

    // Convertir a int después de la escala y asegurar que los valores estén dentro del rango de 0 a new_maxlevel
    pixel_data.r[i] = static_cast<uint16_t>(std::clamp<int>(static_cast<int>(r_scaled), 0, new_maxlevel));
    pixel_data.g[i] = static_cast<uint16_t>(std::clamp<int>(static_cast<int>(g_scaled), 0, new_maxlevel));
    pixel_data.b[i] = static_cast<uint16_t>(std::clamp<int>(static_cast<int>(b_scaled), 0, new_maxlevel));
  }

  // Actualizar max_color al nuevo nivel máximo
  header.max_color = new_maxlevel;
}




