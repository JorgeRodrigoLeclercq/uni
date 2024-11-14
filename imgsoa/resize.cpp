
#include "resize.hpp"

#include "imgaos/info.hpp"

#include <algorithm>  // For std::clamp
#include <cmath>
#include <fstream>

void ReSize ( ImageHeader header, SoA & pixel_data , const ImageDimensions new_dimensions, std::ofstream &output) {

  const auto new_pixel_count = static_cast<unsigned long>(new_dimensions.width )* static_cast<unsigned long> (new_dimensions.height);

  bool const is_16_bit = header.max_color > DEFAULT_MAX_COLOR;
  // Structure of Arrays
  SoA new_pixel_data;
  new_pixel_data.r.resize(new_pixel_count);
  new_pixel_data.g.resize(new_pixel_count);
  new_pixel_data.b.resize(new_pixel_count);

  PixelCalculator(header, pixel_data, new_dimensions , new_pixel_data);
  header.dimensions.width = new_dimensions.width;
  header.dimensions.height = new_dimensions.height;

  write_info( output, header , new_pixel_data, is_16_bit);

}

void PixelCalculator(  const ImageHeader & header,  SoA & pixel_Data,
                     const ImageDimensions & new_dimension, SoA &new_pixel_data) {
  ImageDimensions original_dimension{};
  original_dimension.width = header.dimensions.width;
  original_dimension.height = header.dimensions.height;

  std::vector<float> coordinates = {0,0,0,0,0,0};

//hacer prueba de separar los for
  for ( int i = 0; i < new_dimension.height; i++ ) {
    for ( int j = 0; j < new_dimension.width; j++ ) {
      coordinates = coordinates_calculator(i, new_dimension , j , original_dimension );
      new_pixel_data.r[static_cast<unsigned long int>(std::abs((i * new_dimension.width) + j))] = interpolacion_colores_red(pixel_Data,coordinates,original_dimension);
      new_pixel_data.g[static_cast<unsigned long int>(std::abs((i * new_dimension.width) + j))] = interpolacion_colores_blue(pixel_Data,coordinates,original_dimension);
      new_pixel_data.b[static_cast<unsigned long int>(std::abs((i * new_dimension.width) + j))] = interpolacion_colores_green(pixel_Data,coordinates,original_dimension);

    }
  }

}

uint16_t interpolacion_colores_red ( const SoA &pixel_Data, const std::vector<float> &coordinates ,  const ImageDimensions &original_dimension ) {
  uint16_t pixel_left_down = 0;
  uint16_t pixel_right_down = 0;
  uint16_t pixel_left_up = 0;
  uint16_t pixel_right_up = 0;
  uint16_t color1 = 0;
  uint16_t color2 = 0;
  float fraction = coordinates[0] - coordinates[1];

  pixel_left_down = pixel_Data.r[static_cast<unsigned long long int>((coordinates[1] * static_cast<float>(original_dimension.width)) + coordinates[4])];
  pixel_right_down = pixel_Data.r[static_cast<unsigned long long int>((coordinates[2] * static_cast<float>(original_dimension.width)) + coordinates[4])];
  pixel_left_up = pixel_Data.r[static_cast<unsigned long long int>((coordinates[1]  * static_cast<float>(original_dimension.width)) + coordinates[4 +1])];
  pixel_right_up = pixel_Data.r[static_cast<unsigned long long int>((coordinates[2] * static_cast<float>(original_dimension.width)) + coordinates[4 + 1])];

  color1 = interpolacion_correcta_colores(pixel_right_down, fraction, pixel_left_down);
  color2 = interpolacion_correcta_colores(pixel_left_up, fraction, pixel_right_up);

  fraction= coordinates[3]-  coordinates[4 ];
  return interpolacion_correcta_colores(color1, fraction, color2);

}


uint16_t interpolacion_colores_green ( const SoA &pixel_Data, const std::vector<float> &coordinates ,  const ImageDimensions &original_dimension ) {
  uint16_t pixel_left_down = 0;
  uint16_t pixel_right_down = 0;
  uint16_t pixel_left_up = 0;
  uint16_t pixel_right_up = 0;
  uint16_t color1 = 0;
  uint16_t color2 = 0;
  float fraction = coordinates[0] - coordinates[1];

  pixel_left_down = pixel_Data.g[static_cast<unsigned long long int>((coordinates[1] * static_cast<float>(original_dimension.width)) + coordinates[4])];
  pixel_right_down = pixel_Data.g[static_cast<unsigned long long int>((coordinates[2] * static_cast<float>(original_dimension.width)) + coordinates[4])];
  pixel_left_up = pixel_Data.g[static_cast<unsigned long long int>((coordinates[1]  * static_cast<float>(original_dimension.width)) + coordinates[4 +1])];
  pixel_right_up = pixel_Data.g[static_cast<unsigned long long int>((coordinates[2] * static_cast<float>(original_dimension.width)) + coordinates[4 + 1])];

  color1 = interpolacion_correcta_colores(pixel_left_down, fraction , pixel_right_down);
  color2 = interpolacion_correcta_colores(pixel_left_up, fraction, pixel_right_up);

  fraction= coordinates[3] -  coordinates[4 ];
  return interpolacion_correcta_colores(color1, fraction, color2);

}


uint16_t interpolacion_colores_blue( const SoA &pixel_Data, const std::vector<float> &coordinates ,  const ImageDimensions &original_dimension ) {
  uint16_t pixel_left_down = 0;
  uint16_t pixel_right_down = 0;
  uint16_t pixel_left_up = 0;
  uint16_t pixel_right_up = 0;
  uint16_t color1 = 0;
  uint16_t color2 = 0;
  float fraction = coordinates[0] - coordinates[1];


  pixel_left_down = pixel_Data.b[static_cast<unsigned long long int>((coordinates[1] * static_cast<float>(original_dimension.width)) + coordinates[4])];
  pixel_right_down = pixel_Data.b[static_cast<unsigned long long int>((coordinates[2] * static_cast<float>(original_dimension.width)) + coordinates[4])];
  pixel_left_up = pixel_Data.b[static_cast<unsigned long long int>((coordinates[1]  * static_cast<float>(original_dimension.width)) + coordinates[4 +1])];
  pixel_right_up = pixel_Data.b[static_cast<unsigned long long int>((coordinates[2] * static_cast<float>(original_dimension.width)) + coordinates[4 + 1])];

  color1 = interpolacion_correcta_colores(pixel_left_down, fraction, pixel_right_down);
  color2 = interpolacion_correcta_colores(pixel_left_up, fraction, pixel_right_up);

  fraction = coordinates[3] - coordinates[4];
  return interpolacion_correcta_colores(color1, fraction, color2);

}

uint16_t interpolacion_correcta_colores(const uint16_t &left_up, const float fraction ,const uint16_t &right_up) {

  auto color =static_cast<uint16_t>( (static_cast<float>(right_up - left_up) * fraction ) + static_cast<float>(left_up));

  return color;

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