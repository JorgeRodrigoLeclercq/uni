#include "functions.hpp"
#include <cmath>
#include <bits/algorithmfwd.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>

double interpolacion(const std::vector<double>  &first_point , const std::vector<double> & second_point , const int y_value) {
   //Formula for getting the z ( color) value of the interpolation of two thredimensional points
      return ( first_point[2] + (( second_point[2] - first_point[2]) * ((y_value - first_point[1]) / ( second_point[1] - first_point[0]))));

}

Pixel interpolacion_colores ( const std::vector<Pixel> &pixel_Data, std::vector<double> &coordenadas , const int width_counter , const ImageDimensions &original_dimension ) {

  Pixel pixel;
              std::vector<double> first_point = {coordenadas[1], coordenadas[4], (double)pixel_Data[static_cast<unsigned long long int>(
                            static_cast<long>(coordenadas[1] + coordenadas[4]) *
                            original_dimension.width)].channels.red};
              std::vector<double> second_point = {coordenadas[2], coordenadas[4], (double)pixel_Data[static_cast<unsigned long long int>(
                                        static_cast<long>(coordenadas[2] + coordenadas[4]) *
                                        original_dimension.width)].channels.red};
              double color_c1 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[1], coordenadas[4+1],static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                            static_cast<long>(coordenadas[1] + coordenadas[4+1]) *
                                            original_dimension.width)].channels.red)};
              second_point = {coordenadas[2], coordenadas[4 +1], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                             static_cast<long>(coordenadas[2] + coordenadas[4 +1]) *
                                             original_dimension.width)].channels.red)};
              double color_c2 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[0], coordenadas[4], color_c1};
              second_point = {coordenadas[0],coordenadas[4 +1], color_c2};
              pixel.channels.red      = static_cast<uint16_t>(interpolacion(first_point, second_point, width_counter));
              // --g
              first_point = {coordenadas[1], coordenadas[4], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                            static_cast<long>(coordenadas[1] + coordenadas[4]) *
                                            original_dimension.width)].channels.green)};
              second_point = {coordenadas[2], coordenadas[4], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                             static_cast<long>(coordenadas[2] + coordenadas[4]) *
                                             original_dimension.width)].channels.green)};
              color_c1 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[1], coordenadas[4 +1 ], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                            static_cast<long>(coordenadas[1] + coordenadas[4 + 1]) *
                                            original_dimension.width)].channels.green)};
              second_point = {coordenadas[2], coordenadas[4 + 1], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                             static_cast<long>(coordenadas[2] + coordenadas[4 +1]) *
                                             original_dimension.width)].channels.green)};
              color_c2 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[0], coordenadas[4], color_c1};
              second_point = {coordenadas[0], coordenadas[4 +1], color_c2};
              pixel.channels.green = static_cast<uint16_t>(interpolacion(first_point, second_point, width_counter));
              // --b
              first_point = {coordenadas[1], coordenadas[4], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                            static_cast<long>(coordenadas[1] + coordenadas[4]) *
                                            original_dimension.width)].channels.blue)};
              second_point = {coordenadas[2], coordenadas[4], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                             static_cast<long>(coordenadas[2] + coordenadas[4]) *
                                             original_dimension.width)].channels.blue)};
              color_c1 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[1], coordenadas[4+1], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                            static_cast<long>(coordenadas[1] + coordenadas[4+ 1]) *
                                            original_dimension.width)].channels.blue)};
              second_point = {coordenadas[2], coordenadas[4+1], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                             static_cast<long>(coordenadas[2] + coordenadas[4+ 1]) *
                                             original_dimension.width)].channels.blue)};
              color_c2 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[0], coordenadas[4], color_c1};
              second_point = {coordenadas[0], coordenadas[4+1], color_c2};
              pixel.channels.blue = static_cast<uint16_t>(interpolacion(first_point, second_point, width_counter));

  return pixel;

}

  void DimensionChange(const ImageDimensions& original_dimension, const  std::vector<Pixel> &pixel_Data , const ImageDimensions& new_dimension) {
      std::vector<Pixel> new_pixel_data(static_cast<std::size_t>(new_dimension.width * new_dimension.height));
      Pixel pixel; pixel.channels.red = 0; pixel.channels.green = 0; pixel.channels.blue = 0;
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
              new_pixel_data[static_cast<unsigned long int>(j + (i * new_dimension.width))] = interpolacion_colores(pixel_Data, coordenadas, i , original_dimension);
            }
      }
}