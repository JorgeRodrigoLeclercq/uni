#include "../common/header.hpp"
#include "../common/progargs.hpp"
#include "../common/pixel_structures.hpp"
#include "imgaos/cutfreq.hpp"
#include "imgaos/info.hpp"
#include "imgaos/maxlevel.hpp"
#include "imgaos/resize.hpp"
#include "imgaos/compress.hpp"

#include <cstdlib>
#include <fstream>
#include <gsl/gsl>
#include <iostream>
#include <string>
#include <vector>

constexpr uint8_t MAX_COLOR_VALUE8 = 255;

int main(int argc, const char *argv[]) {
  checkNumberArgs(argc);
  gsl::span const args{argv, gsl::narrow<std::size_t>(argc)};
  std::ifstream infile(args[1], std::ios::binary);
  std::ofstream outfile(args[2], std::ios::binary);
  ImageHeader header;
  get_header(infile, header);
  auto pixel_count = static_cast<unsigned long long int>(header.dimensions.width) *
                   static_cast<unsigned long long int>(header.dimensions.height);
  std::vector<Pixel> pixel_data(static_cast<std::size_t>(pixel_count));
  bool is_16_bit = header.max_color > MAX_COLOR_VALUE8;  // determinar la longitud de cada pixel (2 bytes si max_color > 256; else: 1)
  get_pixels(infile, pixel_data, pixel_count, is_16_bit);  // rellenar el Array of Structures con los píxeles
  if (args[3] == std::string("info")) {
    checkInfoAndCompress(argc);
    info(header, pixel_data);
    write_info(outfile, header, pixel_data, is_16_bit);
  } else if (args[3] == std::string("maxlevel")) {
    int const new_maxlevel = checkMaxLevel(args[4]);
    gsl::span<Pixel> pixel_span{pixel_data};
    maxlevel(new_maxlevel, is_16_bit, pixel_span, header);
    write_info(outfile, header, pixel_data, is_16_bit);
  }else if (args[3] == std::string("resize")){
    const ImageDimensions new_dimensions{.width=std::stoi(args[4]),.height=std::stoi(args[EXTRA_ARGS])};
    checkDimensions(new_dimensions);
    ReSize(header, pixel_data, new_dimensions, outfile);
  } else if (args[3] == std::string("cutfreq") && argc == EXTRA_ARGS){
    int const n_colors = checkCutFreq(args, argc);
    cutfreq(pixel_data, n_colors);
    write_info(outfile, header, pixel_data, is_16_bit);
  } else if (args[3] == std::string("compress")){
    checkInfoAndCompress(argc);
    compress(outfile, header, pixel_data);
  } else {
    std::cerr << "Error: Invalid option: " << args[3] << "\n";
    exit(-1);
  }
  return 0;
};