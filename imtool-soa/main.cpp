#include "functions.hpp"
#include "common/progargs.hpp"
#include "common/binaryio.hpp"

#include <gsl/gsl>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {

  checkNumberArgs(argc); // Comprobar que el número de argumentos es correcto

  gsl::span const args{argv, gsl::narrow<std::size_t>(argc)}; // Creamos la vista

  std::ifstream infile(args[1], std::ios::binary);
  std::ofstream outfile(args[2], std::ios::binary);

  // Variables del header
  std::string magic_number;
  int width;
  int height;
  int max_color;

  // INFO COMMAND
  get_header(infile, magic_number, width, height, max_color);

  // Tamaño en pixeles de la imagen
  int pixel_count = width * height;

  // Structure of Arrays
  SoA pixel_data;
  pixel_data.r.resize(pixel_count);
  pixel_data.g.resize(pixel_count);
  pixel_data.b.resize(pixel_count);

  // Determinar la longitud de cada pixel (2 bytes si max_color > 256; else: 1)
  bool is_16_bit = max_color > 255;

  // // RELLENAR LOS ARRAYS DE LA ESTRUCTURA CON LOS PIXELES
  get_pixels(infile, pixel_data, pixel_count, is_16_bit);

  // Escribir en outfile, INFO COMMAND
  write_info(outfile, magic_number, width, height, max_color, pixel_data, is_16_bit);

  std::ofstream cppm_outfile(args[3], std::ios::binary);

  // COMPRESS COMMAND
  write_cppm(cppm_outfile, pixel_data, width, height, max_color);

  infile.close();
  outfile.close();
  cppm_outfile.close();
  return 0;
}
