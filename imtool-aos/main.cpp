#include "../common/header.hpp"
#include "../common/progargs.hpp"
#include "common/pixel_structures.hpp"
#include "cutfreq.hpp"
#include "info.hpp"
#include "maxlevel.hpp"
#include "resize.hpp"

#include <cstdlib>
#include <fstream>
#include <gsl/gsl>
#include <iostream>
#include <string>
#include <vector>

constexpr uint8_t MAX_COLOR_VALUE8 = 255;
constexpr uint8_t EXTRA_ARGS = 5;

int main(int argc, const char *argv[]) {

  // Comprobar que el número de argumentos es correcto para todas las funciones
  checkNumberArgs(argc);
  gsl::span const args{argv, gsl::narrow<std::size_t>(argc)};

  // Abrir archivos de entrada y de salida
  std::ifstream infile(args[1], std::ios::binary);
  std::ofstream outfile(args[2], std::ios::binary);

  // Extramos el header del archivo
  ImageHeader header;
  get_header(infile, header);

  // Tamaño en pixeles de la imagen
  auto pixel_count = static_cast<unsigned long long int>(header.dimensions.width) *
                   static_cast<unsigned long long int>(header.dimensions.height);

  // Array of Structures
  std::vector<Pixel> pixel_data(static_cast<std::size_t>(pixel_count));
  bool is_16_bit = header.max_color > MAX_COLOR_VALUE8;  // determinar la longitud de cada pixel (2 bytes si max_color > 256; else: 1)
  get_pixels(infile, pixel_data, pixel_count, is_16_bit);  // rellenar el Array of Structures con los píxeles

  if (args[3] == std::string("maxlevel")) {
    int const new_maxlevel = checkMaxLevel(args[4]);
    gsl::span<Pixel> pixel_span{pixel_data};
    maxlevel(new_maxlevel, is_16_bit, pixel_span, header);
  }
  else if (args[3] == std::string("resize")){
    checkNumberArgs(argc);

    ImageDimensions new_dimensions{};
    new_dimensions.width= std::stoi(args[4]);
    new_dimensions.width= std::stoi(args[4+1]);

    checkHeightArgs(new_dimensions.height);
    checkWidthArgs(new_dimensions.width); checkNumberArgs(argc);

    ReSize(header, pixel_data, new_dimensions, outfile);
  }
  else if (args[3] == std::string("cutfreq") && argc == EXTRA_ARGS){
    try{cutfreq(pixel_data, std::stoi(args[4]));}
    catch (const std::invalid_argument &){
      std::cerr << "Error: Invalid cutfreq: " << args[4] << "\n";
      exit(-1);
    };
  }
  else if (args[3] == std::string("compress")){
    //write_cppm(outfile, header, pixel_data);
    return 0;
  }
  else if (args[3] != std::string("info")){
    std::cerr << "Error: Invalid option: " << args[3] << "\n";
    exit(-1);
  }

  write_info(outfile, header, pixel_data, is_16_bit);  // escribimos la nueva información en el arhcivo de salida

  return 0;
};