#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <algorithm>
#include <cstdint>
#include "./imtool-soa/functions.hpp"
#include <string>
#include <cmath>
#include <utility>
#include <vector>


constexpr int DEFAULT_MAX_COLOR = 255;
constexpr uint8_t MAX_COLOR_VALUE8 = 255;
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

void ReSize ( ImageHeader header, SoA & pixel_data , ImageDimensions new_dimensions, std::ofstream &output);

void PixelCalculator(  const ImageHeader & header,  SoA & pixel_Data,
                     const ImageDimensions & new_dimension, SoA &new_pixel_data);
#endif //RESIZE_HPP
