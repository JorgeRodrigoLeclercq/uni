#include "../common/header.hpp"
#include "../common/progargs.hpp"
#include "common/pixel_structures.hpp"
#include "imgaos/cutfreq.hpp"
#include "imgaos/info.hpp"
#include "imgaos/maxlevel.hpp"
#include "imgaos/resize.hpp"

#include <cstdlib>
#include <fstream>
#include <gsl/gsl>
#include <iostream>
#include <string>
#include <vector>

constexpr uint8_t MAX_COLOR_VALUE8 = 255;

int main(int argc, const char *argv[]) {

  if (argc < 3) {
    std::cerr << "Error: Invalid number of arguments: " << argc - 1 << "\n";
    exit(-1); // Código de error si hay menos de 3 argumentos
  }
  gsl::span const args{argv, gsl::narrow<std::size_t>(argc)};
  std::ifstream file1(args[1], std::ios::binary);
  std::ifstream file2(args[2], std::ios::binary);
  ImageHeader header;
  get_header(file1, header);
  auto pixel_count = static_cast<unsigned long long int>(header.dimensions.width) *
                   static_cast<unsigned long long int>(header.dimensions.height);
  std::vector<Pixel> pixel_data_1(static_cast<std::size_t>(pixel_count));
  bool is_16_bit = header.max_color > MAX_COLOR_VALUE8;  // determinar la longitud de cada pixel (2 bytes si max_color > 256; else: 1)
  get_pixels(file1, pixel_data_1, pixel_count, is_16_bit);  // rellenar el Array of Structures con los píxeles


  get_header(file1, header);
  pixel_count = static_cast<unsigned long long int>(header.dimensions.width) *
                   static_cast<unsigned long long int>(header.dimensions.height);
  std::vector<Pixel> pixel_data_2(static_cast<std::size_t>(pixel_count));
  is_16_bit = header.max_color > MAX_COLOR_VALUE8;  // determinar la longitud de cada pixel (2 bytes si max_color > 256; else: 1)
  get_pixels(file1, pixel_data_2, pixel_count, is_16_bit);

  for( uint i = 0; i < pixel_count; i++) {

    if ( std::abs(pixel_data_1[i].channels.red - pixel_data_2[i].channels.red) > 5){
      std::cerr << "Error: Images red color dont match."  << "\n";
      exit(-1); // Código de error si hay una diferencia de 5 puntos entre los valores
    }
    if ( std::abs(pixel_data_1[i].channels.blue- pixel_data_2[i].channels.blue) > 5){
      std::cerr << "Error: Images blue color dont match."  << "\n";
      exit(-1); // Código de error si hay una diferencia de 5 puntos entre los valores
    }
    if ( std::abs(pixel_data_1[i].channels.green - pixel_data_2[i].channels.green) > 5){
      std::cerr << "Error: Images green color dont match."  << "\n";
      exit(-1); // Código de error si hay una diferencia de 5 puntos entre los valores
    }
  }
  exit(0);

};