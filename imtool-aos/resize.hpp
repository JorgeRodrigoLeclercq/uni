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

double interpolacion(const std::vector<double> &first_point, const std::vector<double> &second_point, int y_value);

Pixel interpolacion_colores ( const std::vector<Pixel> &pixel_Data, std::vector<double> &coordenadas ,  int width_counter , const ImageDimensions &original_dimension );

void DimensionChange(const ImageDimensions& original_dimension, const  std::vector<Pixel> &pixel_Data , const ImageDimensions& new_dimension,  std::vector<Pixel> &new_pixel_data);
#endif //RESIZE_HPP
