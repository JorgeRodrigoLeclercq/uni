#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <gsl/span>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

constexpr int DEFAULT_MAX_COLOR = 255;
struct ImageDimensions {
  int width;
  int height;
};

// Structure of Arrays (SoA)
struct SoA {
  std::vector<uint16_t> r, g, b;  // arrays distintos para el rojo, el verde y el azul
};

struct ImageHeader {
  std::string magic_number;
  ImageDimensions dimensions;
  int max_color;

  ImageHeader(std::string magic = "", ImageDimensions dims = {0, 0}, int max_c = DEFAULT_MAX_COLOR)
    : magic_number(std::move(magic)), dimensions(dims), max_color(max_c) {}
};


double interpolacion(const std::vector<double> &first_point, const std::vector<double> &second_point, int y_value);

std::vector<uint16_t> interpolacion_colores ( const SoA &pixel_Data, const std::vector<double> &coordenadas ,  int width_counter , const ImageDimensions &original_dimension ) ;


SoA ReSize( ImageDimensions & original_dimension,  SoA & pixel_Data,
                     const ImageDimensions & new_dimension) ;
#endif //RESIZE_HPP
