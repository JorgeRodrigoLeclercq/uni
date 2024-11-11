
#include "ReSize.hpp"

#include <algorithm>  // For std::clamp
#include <bits/algorithmfwd.h>
#include <cmath>
#include <fstream>
#include <iostream>

SoA ReSize( ImageDimensions & original_dimension,  SoA & pixel_Data,
                     const ImageDimensions & new_dimension) {
  const auto pixel_count = static_cast<uint>( std::abs(new_dimension.width * new_dimension.height));

  // Structure of Arrays
  SoA new_pixel_data;
  new_pixel_data.r.resize(pixel_count);
  new_pixel_data.g.resize(pixel_count);
  new_pixel_data.b.resize(pixel_count);
  double new_x =  0.0;
  double x_floor = 0.0;
  double x_ceil = 0.0;
  double new_y = 0.0;
  double y_floor = 0.0;
  double y_ceil = 0.0;
  std::vector<double> coordenadas = {new_x, x_floor, x_ceil, new_y , y_floor, y_ceil};

  for ( int i = 0; i < new_dimension.height; i++ ) {
    for ( int j = 0; j < new_dimension.width; j++ ) {
      new_x = static_cast<double>( j * original_dimension.width) / static_cast<double>(new_dimension.width);
      x_floor = std::floor(new_x);
      x_ceil = std::ceil(new_x);
      new_y =  static_cast<double>( i * original_dimension.height) / static_cast<double>(new_dimension.height);
      y_floor = std::floor(new_y);
      y_ceil = std::ceil(new_y);

      coordenadas = {new_x, x_floor, x_ceil, new_y , y_floor, y_ceil};
      //We write the new data in our new image
      std::vector<uint16_t> new_data = interpolacion_colores(pixel_Data, coordenadas, i , original_dimension);
      new_pixel_data.r[static_cast<unsigned long long int >(j + (i * new_dimension.width))] = new_data[0];
      new_pixel_data.g[static_cast<unsigned long long int> (j  + (i * new_dimension.width))] = new_data[1];
      new_pixel_data.b[static_cast<unsigned long long int> (j  + (i * new_dimension.width))] = new_data[2];
    }
  }
  return new_pixel_data;


}

std::vector<uint16_t> interpolacion_colores ( const SoA &pixel_Data, const std::vector<double> &coordenadas , const int width_counter , const ImageDimensions &original_dimension ) {
              std::vector<uint16_t> new_colors ;
              std::vector<double> first_point = {0,0,0};
              std::vector<double> second_point = {0,0,0};
              double color_c1 = 0.0;
              double color_c2 = 0.0;

                first_point = {coordenadas[1], coordenadas[4], static_cast<double>(pixel_Data.r[static_cast<unsigned long long int>(
                            static_cast<long>(coordenadas[1] + coordenadas[4]) *
                            original_dimension.width)])};
                second_point = {coordenadas[2], coordenadas[4], static_cast<double>(pixel_Data.r[static_cast<unsigned long long int>(
                                          (coordenadas[2] + coordenadas[4]) *
                                          original_dimension.width)])};
                color_c1 = interpolacion(first_point, second_point, width_counter);
                first_point = {coordenadas[1], coordenadas[ 4 + 1 ],static_cast<double>(pixel_Data.r[static_cast<unsigned long long int>(
                                              (coordenadas[1] + coordenadas[4 + 1]) *
                                              original_dimension.width)])};
                second_point = {coordenadas[2], coordenadas[4 + 1], static_cast<double>(pixel_Data.r[static_cast<unsigned long long int>(
                                               (coordenadas[2] + coordenadas[4 + 1]) *
                                               original_dimension.width)])};
                color_c2 = interpolacion(first_point, second_point, width_counter);
                first_point = {coordenadas[0], coordenadas[4], color_c1};
                second_point = {coordenadas[0],coordenadas[4 + 1], color_c2};

                new_colors.push_back( static_cast<uint16_t>(interpolacion(first_point, second_point, width_counter)));



               return new_colors;

}

double interpolacion( const std::vector<double>  &first_point ,const std::vector<double> &second_point , const  int y_value) {
      //Formula for getting the z ( color) value of the interpolation of two thredimensional points
      return ( first_point[2] + (( second_point[2] - first_point[2]) * ((y_value - first_point[1]) / ( second_point[1] - first_point[0]))));

    }