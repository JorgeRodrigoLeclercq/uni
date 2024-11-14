#include "resize.hpp"

#include <algorithm>
#include <bits/algorithmfwd.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>

void ReSize ( ImageHeader header,  const std::vector<Pixel> &pixel_Data, const ImageDimensions new_dimensions, std::ofstream &output) {

  ImageDimensions old_dimensions{};
  old_dimensions.width = header.dimensions.width;
  old_dimensions.height = header.dimensions.height;
  const int pixel_count = new_dimensions.width * new_dimensions.height;
  std::vector<Pixel> new_pixels(static_cast<std::size_t>(pixel_count));


  bool const is_16_bit = header.max_color > DEFAULT_MAX_COLOR;
  // Structure of Arrays


  DimensionChange(old_dimensions ,pixel_Data, new_dimensions , new_pixels);
  header.dimensions.width = new_dimensions.width;
  header.dimensions.height = new_dimensions.height;

  write_info( output, header , new_pixels, is_16_bit);

}


Pixel interpolacion_correcta_colores(const Pixel &left_up, const float fraction ,const Pixel &right_up) {
  Pixel pixel;
  pixel.channels.red =static_cast<uint16_t>( (static_cast<float>(right_up.channels.red - left_up.channels.red ) * fraction ) + static_cast<float>(left_up.channels.red));
  pixel.channels.blue =static_cast<uint16_t>( (static_cast<float>(right_up.channels.blue - left_up.channels.blue ) * fraction ) + static_cast<float>(left_up.channels.blue));
  pixel.channels.green =static_cast<uint16_t>( (static_cast<float>(right_up.channels.green- left_up.channels.green ) * fraction ) + static_cast<float>(left_up.channels.green));

  return pixel;

}


  void DimensionChange(const ImageDimensions& original_dimension, const  std::vector<Pixel> &pixel_Data ,
    const ImageDimensions& new_dimension,  std::vector<Pixel> &new_pixel_data) {

      Pixel pixel_left_down{};
      Pixel pixel_right_down{};
      Pixel pixel_left_up{};
      Pixel pixel_right_up{};
      Pixel color1{};
      Pixel color2{};
      float fraction = 0.0;
      std::vector<float> coordinates = {0,0,0,0,0,0};

      for ( int j = 0; j < new_dimension.height; j++ ) {
            for ( int i = 0; i < new_dimension.width; i++ ) {
              coordinates = coordinates_calculator(i, new_dimension , j , original_dimension );

              fraction= coordinates [0] - coordinates[1] ;
              //fraction = 0.5;

              pixel_left_down = pixel_Data[static_cast<unsigned long long int>((coordinates[4] * static_cast<float>(original_dimension.width)) + coordinates[1])];
              pixel_right_down = pixel_Data[static_cast<unsigned long long int>((coordinates[4] * static_cast<float>(original_dimension.width)) + coordinates[2])];
              pixel_left_up = pixel_Data[static_cast<unsigned long long int>((coordinates[4+1]  * static_cast<float>(original_dimension.width)) + coordinates[1])];
              pixel_right_up = pixel_Data[static_cast<unsigned long long int>((coordinates[4+1] * static_cast<float>(original_dimension.width)) + coordinates[2])];

              color1 = interpolacion_correcta_colores(pixel_right_down, fraction, pixel_left_down);

              //fraction= coordinates [2] - coordinates[0];
              color2 = interpolacion_correcta_colores(pixel_left_up, fraction, pixel_right_up);

              fraction= coordinates [3] - coordinates[4] ;
              new_pixel_data[static_cast<unsigned long int>(std::abs((j * new_dimension.width) + i))] = interpolacion_correcta_colores(color1, fraction, color2);



            }
      }
}

std::vector<float> coordinates_calculator(int const x_coordinate , const ImageDimensions &new_dimension, const int y_coordinate,const ImageDimensions &original_dimension) {

  std::vector<float> coordinates{0,0,0,0,0,0};

  coordinates[0]= static_cast<float>( x_coordinate * original_dimension.width) / static_cast<float>(new_dimension.width);
  coordinates[1] = std::floor(coordinates[0]);
  coordinates[2]= std::ceil(coordinates[0]);
  coordinates[3] =  static_cast<float>( y_coordinate * original_dimension.height) / static_cast<float>(new_dimension.height);
  coordinates[4] = std::floor(coordinates[3]);
  coordinates[4 +1] = std::ceil(coordinates[3]);

  return coordinates;

}