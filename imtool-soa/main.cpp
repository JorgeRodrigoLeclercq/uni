#include "functions.hpp"

#include <gsl/gsl>
#include <fstream>
#include <iostream>
#include <span>

int main(int argc, char *argv[]) {

  gsl::span const args{argv, gsl::narrow<std::size_t>(argc)};

  if (argc < 2) {
      std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
      return 1;
  }

  std::ifstream infile(argv[1], std::ios::binary);  // abrir en modo binario
  if (!infile) {
      std::cerr << "Error: Could not open file " << argv[1] << std::endl;
      return 1;
  }

  std::ofstream outfile(argv[2], std::ios::binary);

  // Variables del header
  std::string magic_number;
  int width, height, max_color;

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

  std::ofstream cppm_outfile(argv[3], std::ios::binary);

  // COMPRESS COMMAND
  write_cppm(cppm_outfile, pixel_data, width, height, max_color);

  scale_intensity(pixel_data, 1.2f);
  infile.close();
  outfile.close();
  cppm_outfile.close();
  return 0;


  return 0;
}
