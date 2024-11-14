#include "../common/header.hpp"
#include "../common/progargs.hpp"
#include "imgsoa/cutfreq.hpp"
#include "imgsoa/info.hpp"
#include "imgsoa/maxlevel.hpp"
#include "imgsoa/resize.hpp"

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <gsl/gsl>
#include <iostream>
#include <string>

constexpr uint8_t MAX_COLOR_VALUE8 = 255;

int main(int argc, const char *argv[]) {
  auto inicio = std::chrono::high_resolution_clock::now();
  checkNumberArgs(argc);
  gsl::span const args{argv, gsl::narrow<std::size_t>(argc)};
  std::ifstream infile(args[1], std::ios::binary);
  std::ofstream outfile(args[2], std::ios::binary);
  ImageHeader header;
  get_header(infile, header);
  auto pixel_count = static_cast<unsigned long long int>(header.dimensions.width) *
                   static_cast<unsigned long long int>(header.dimensions.height);
  SoA pixel_data;
  pixel_data.resize(static_cast<std::size_t>(pixel_count));
  bool is_16_bit = header.max_color > MAX_COLOR_VALUE8;  // determinar la longitud de cada pixel (2 bytes si max_color > 256; else: 1)
  get_pixels(infile, pixel_data, pixel_count, is_16_bit);  // rellenar el Structure of Arrays con los píxeles
  if (args[3] == std::string("maxlevel")) {
    int const new_maxlevel = checkMaxLevel(args[4]);
    maxlevel(new_maxlevel, is_16_bit, pixel_data, header);
  }
  else if (args[3] == std::string("resize")) {
    const ImageDimensions new_dimensions{.width=std::stoi(args[4]),.height=std::stoi(args[EXTRA_ARGS])};
    checkDimensions(new_dimensions);
    ReSize(header, pixel_data,new_dimensions , outfile);
  }
  else if (args[3] == std::string("cutfreq") && argc == EXTRA_ARGS){
    int const n_colors = checkCutFreq(args, argc);
    cutfreq(pixel_data, n_colors);
  }
  else if (args[3] == std::string("compress")){
    //write_cppm(outfile, header, pixel_data);
  }
  else {
    std::cerr << "Error: Invalid command: " << args[3] << "\n";
    exit(-1);
  }
  write_info(outfile, header, pixel_data, is_16_bit);  // escribimos la nueva información en el arhcivo de salida
  auto fin = std::chrono::high_resolution_clock::now();
  auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio).count();
  std::cout << "Tiempo para calcular los mas cercanos: " << duracion << " microsegundos" << "\n";
  return 0;
};