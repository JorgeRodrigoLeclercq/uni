#include "../common/progargs.hpp"
#include "functions.hpp"

#include <cstdlib>
#include <fstream>
#include <gsl/gsl>
#include <iostream>
#include <string>
#include <vector>

constexpr uint8_t MAX_COLOR_VALUE8 = 255;
constexpr uint16_t MAX_COLOR_VALUE16 = 65535;

int main(int argc, const char *argv[]) {

  // Comprobar que el número de argumentos es correcto para todas las funciones
  checkNumberArgs(argc);
  gsl::span const args{argv, gsl::narrow<std::size_t>(argc)};

  // Abrir archivos de entrada y de salida
  std::ifstream infile(args[1], std::ios::binary);
  if (!infile) {
    std::cerr << "Error: Could not open file " << args[2] << "\n";
    return 1;
  }

  std::ofstream outfile(args[2], std::ios::binary);
  if (!outfile) {
    std::cerr << "Error: Could not open file " << args[2] << "\n";
    infile.close();
    return 1;
  }

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

  if (args[3] == "maxlevel"){
    // Código para el comando "maxlevel"
    if (argc != 5) {
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
  else if (args[3] == "resize"){
    // Código para el comando "resize"
  }
  else if (args[3] == "cutfreq"){
    // Código para el comando "cutfreq"
  }
  else if (args[3] == "compress"){
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