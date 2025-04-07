#ifndef RESIZE_HPP
#define RESIZE_HPP

#include "info.hpp"

#include <cstdint>
#include <vector>

double interpolacion(const std::vector<double> &first_point, const std::vector<double> &second_point, int y_value);

uint16_t interpolacion_colores_red ( const SoA &pixel_Data, const std::vector<float> &coordinates ,  const ImageDimensions &original_dimension );


uint16_t interpolacion_colores_blue ( const SoA &pixel_Data, const std::vector<float> &coordinates ,  const ImageDimensions &original_dimension );


uint16_t interpolacion_colores_green ( const SoA &pixel_Data, const std::vector<float> &coordinates ,  const ImageDimensions &original_dimension );

void ReSize ( ImageHeader header, SoA & pixel_data , ImageDimensions new_dimensions, std::ofstream &output);

void PixelCalculator(  const ImageHeader & header,  SoA & pixel_Data,
                     const ImageDimensions & new_dimension, SoA &new_pixel_data);

std::vector<float> coordinates_calculator(int x_coordinate , const ImageDimensions &new_dimension,
  int y_coordinate,const ImageDimensions &original_dimension) ;

uint16_t interpolacion_correcta_colores(const uint16_t &left_up, float fraction ,const uint16_t &right_up);
#endif //RESIZE_HPP
