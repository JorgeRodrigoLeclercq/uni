#ifndef RESIZE_HPP
#define RESIZE_HPP

#include "info.hpp"
#include <cstdint>
#include <gsl/gsl>
#include <map>
#include <string>
#include <utility>
#include <vector>



void ReSize ( ImageHeader header,  const std::vector<Pixel> &pixel_Data,  ImageDimensions new_dimensions,std::ofstream &output) ;

void DimensionChange(const ImageDimensions& original_dimension, const  std::vector<Pixel> &pixel_Data , const ImageDimensions& new_dimension,  std::vector<Pixel> &new_pixel_data);


Pixel interpolacion_correcta_colores(const Pixel &left, float fraction ,const Pixel &right);

std::vector<float> coordinates_calculator(int  x_coordinate , const ImageDimensions &new_dimension,  int y_coordinate,const ImageDimensions &original_dimension);
#endif //RESIZE_HPP
