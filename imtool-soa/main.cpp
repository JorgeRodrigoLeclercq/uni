#include "../common/progargs.hpp"
#include "functions.hpp"
#include "cutfreq.hpp"

#include <cstdlib>
#include <fstream>
#include <gsl/gsl>
#include <iostream>
#include <string>
#include <vector>

constexpr uint8_t MAX_COLOR_VALUE8 = 255;
constexpr uint16_t MAX_COLOR_VALUE16 = 65535;
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

  // Structure of Arrays
  SoA pixel_data;
  pixel_data.r.resize(pixel_count);
  pixel_data.g.resize(pixel_count);
  pixel_data.b.resize(pixel_count);
  bool const is_16_bit = header.max_color > MAX_COLOR_VALUE8;  // determinar la longitud de cada pixel (2 bytes si max_color > 256; else: 1)
  get_pixels(infile, pixel_data, pixel_count, is_16_bit);  // rellenar el Structure of Arrays con los píxeles

  if (args[3] == std::string("maxlevel")){
    // Código para el comando "maxlevel"
    if (argc != EXTRA_ARGS) {
      std::cerr << "Error: Invalid number of arguments for maxlevel: " << (argc - 4) << "\n";
      exit(-1);
    }

    int new_maxlevel= 0;

    try {
      new_maxlevel = std::stoi(args[4]);
    } catch (const std::invalid_argument &){
      std::cerr << "Error: Invalid argument for maxlevel: " << args[4] << "\n";
      exit(-1);
    }
    if (new_maxlevel < 0 || new_maxlevel > MAX_COLOR_VALUE16){
      std::cerr << "Error: Invalid maxlevel value: " << new_maxlevel << "\n";
      exit(-1);
    }
  }

  //--------------------------------------------------
  else if (args[3] == std::string("resize")) {
    if (argc != 4 + 1) {
      std::cerr << "Error: Invalid number of extra arguments for resize: " << (argc - 4) << "\n";
      exit(-1);
    }
    ImageDimensions new_dimensions{};
    ImageDimensions old_dimensions{};
    old_dimensions.width = header.dimensions.width;
    old_dimensions.height = header.dimensions.height;

    try {
      new_dimensions.width= std::stoi(args[4]);
    } catch (const std::invalid_argument &){
      std::cerr << "Error: Invalid resize width " << args[4] << "\n";
      exit(-1);
    }
    try {
      new_dimensions.width= std::stoi(args[4+1]);
    } catch (const std::invalid_argument &){
      std::cerr << "Error: Invalid resize heigth" << args[4+1] << "\n";
      exit(-1);
    }

    if (new_dimensions.width < 0 ){
      std::cerr << "Error: Invalid resize width: " << new_dimensions.width << "\n";
      exit(-1);
    }
    else if (new_dimensions.height < 0 ){
      std::cerr << "Error: Invalid resize height: " << new_dimensions.height << "\n";
      exit(-1);
    }

    const auto new_pixel_count = static_cast<unsigned long>(new_dimensions.width * new_dimensions.height);

    // Structure of Arrays
    SoA new_pixel_data;
    new_pixel_data.r.resize(new_pixel_count);
    new_pixel_data.g.resize(new_pixel_count);
    new_pixel_data.b.resize(new_pixel_count);

    new_pixel_data = ReSize(old_dimensions, pixel_data,new_dimensions );
    header.dimensions.height = new_dimensions.height;
    header.dimensions.width = new_dimensions.width;

    write_info(outfile, header, new_pixel_data, is_16_bit);

  }



  else if (args[3] == "cutfreq"){
    try{cutfreq(pixel_data, std::stoi(args[4]));}
    catch (const std::invalid_argument &){
      std::cerr << "Error: Invalid cutfreq: " << args[4] << "\n";
      exit(-1);
    };
  }
  else if (args[3] == std::string("compress")){
    // Código para el comando "compress"
    write_cppm(outfile, header, pixel_data);
    return 0;
  }
  else {
    std::cerr << "Error: Invalid command: " << args[3] << "\n";
    exit(-1);
  }

  write_info(outfile, header, pixel_data, is_16_bit);  // escribimos la nueva información en el arhcivo de salida

  // Cerramos los archivos
  infile.close();
  outfile.close();
  return 0;
};