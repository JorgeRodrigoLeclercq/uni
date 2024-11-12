#ifndef RESIZE_HPP
#define RESIZE_HPP

#include <algorithm>
#include <cstdint>
#include "functions.hpp"
#include <string>
#include <cmath>
#include <utility>
#include <vector>




double interpolacion(const std::vector<double> &first_point, const std::vector<double> &second_point, int y_value);

std::vector<uint16_t> interpolacion_colores ( const SoA &pixel_Data, const std::vector<double> &coordenadas ,  int width_counter , const ImageDimensions &original_dimension ) ;

void ReSize ( ImageHeader header, SoA & pixel_data , ImageDimensions new_dimensions, std::ofstream &output);

void PixelCalculator(  const ImageHeader & header,  SoA & pixel_Data,
                     const ImageDimensions & new_dimension, SoA &new_pixel_data);
#endif //RESIZE_HPP
