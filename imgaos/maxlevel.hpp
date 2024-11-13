//
// Created by fjavi on 11/11/2024.
//
#ifndef MAXLEVEL_HPP
#define MAXLEVEL_HPP

#include "info.hpp" // Asegúrate de que incluye las estructuras SoA e ImageHeader necesarias.
#include <cstdint>       // Para uint16_t

constexpr int MAX_COLOR_8BIT = 255;

// Declaración de la función maxlevel
void maxlevel(int new_maxlevel, bool& is_16_bit, gsl::span<Pixel> &pixel_data, ImageHeader &header);

#endif // MAXLEVEL_HPP
