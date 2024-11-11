#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "functions.hpp"
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



double interpolacion(const std::vector<double> &first_point, const std::vector<double> &second_point, int y_value);

Pixel interpolacion_colores ( const std::vector<Pixel> &pixel_Data, std::vector<double> &coordenadas ,  int width_counter , const ImageDimensions &original_dimension );

void DimensionChange(const ImageDimensions& original_dimension, const  std::vector<Pixel> &pixel_Data , const ImageDimensions& new_dimension);

#endif //RESIZE_HPP
