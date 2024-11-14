#include "info.hpp"
#include "common/binaryio.hpp"
#include <cmath>
#include <fstream>

void get_pixels(std::ifstream &infile, SoA &pixel_data, unsigned long long pixel_count, bool is_16_bit) {
  if (is_16_bit) {
    for (unsigned long long int i = 0; i < pixel_count; ++i) {
      pixel_data.r[i] = read_binary<uint16_t>(infile);
      pixel_data.g[i] = read_binary<uint16_t>(infile);
      pixel_data.b[i] = read_binary<uint16_t>(infile);
    }
  } else {
    for (unsigned long long i = 0; i < pixel_count; ++i) {
      pixel_data.r[i] = read_binary<uint8_t>(infile);
      pixel_data.g[i] = read_binary<uint8_t>(infile);
      pixel_data.b[i] = read_binary<uint8_t>(infile);
    }
  }
}

void write_info(std::ofstream& outfile, const ImageHeader& header, const SoA& pixel_data, bool const is_16_bit) {
  outfile << header.magic_number << "\n";
  outfile << header.dimensions.width << " " << header.dimensions.height << "\n";
  outfile << header.max_color << "\n";

  if (is_16_bit) {
    for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
      write_binary(outfile, pixel_data.r[i]);
      write_binary(outfile, pixel_data.g[i]);
      write_binary(outfile, pixel_data.b[i]);
    }
  } else {
    for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
      write_binary(outfile, static_cast<uint8_t>(pixel_data.r[i]));
      write_binary(outfile, static_cast<uint8_t>(pixel_data.g[i]));
      write_binary(outfile, static_cast<uint8_t>(pixel_data.b[i]));
    }
  }
}