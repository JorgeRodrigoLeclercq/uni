#include "info.hpp"
#include <cmath>
#include <fstream>
#include <iostream>

void get_pixels(std::ifstream &infile, SoA &pixel_data, unsigned long long const pixel_count, bool const is_16_bit) {
  if (is_16_bit) {
    for (unsigned long long int i = 0; i < pixel_count; ++i) {
      constexpr int BIT_SHIFT = 8;
      char red1 = 0;
      char red2 = 0;
      char green1 = 0;
      char green2 = 0;
      char blue1 = 0;
      char blue2 = 0;

      infile.read(&red1, 1);
      infile.read(&red2, 1);
      infile.read(&green1, 1);
      infile.read(&green2, 1);
      infile.read(&blue1, 1);
      infile.read(&blue2, 1);

      pixel_data.r[i] = static_cast<uint16_t>(static_cast<uint8_t>(red1) | (static_cast<uint8_t>(red2) << BIT_SHIFT));
      pixel_data.g[i] = static_cast<uint16_t>(static_cast<uint8_t>(green1) | (static_cast<uint8_t>(green2) << BIT_SHIFT));
      pixel_data.b[i] = static_cast<uint16_t>(static_cast<uint8_t>(blue1) | (static_cast<uint8_t>(blue2) << BIT_SHIFT));
    }
  } else {
    for (unsigned long long i = 0; i < pixel_count; ++i) {
      char red = 0;
      char green = 0;
      char blue = 0;

      infile.read(&red, 1);
      infile.read(&green, 1);
      infile.read(&blue, 1);

      pixel_data.r[i] = static_cast<uint8_t>(red);
      pixel_data.g[i] = static_cast<uint8_t>(green);
      pixel_data.b[i] = static_cast<uint8_t>(blue);
    }
  }
}

void write_info(std::ofstream& outfile, const ImageHeader& header, const SoA& pixel_data, bool const is_16_bit) {
  outfile << header.magic_number << "\n";
  outfile << header.dimensions.width << " " << header.dimensions.height << "\n";
  outfile << header.max_color << "\n";

  if (is_16_bit) {
    for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
      constexpr uint16_t MASK_BYTE = 0xFF;
      constexpr int BITS_PER_BYTE = 8;
      char const red1   = static_cast<char>(pixel_data.r[i] & MASK_BYTE);
      char const red2   = static_cast<char>((pixel_data.r[i] >> BITS_PER_BYTE) & MASK_BYTE);
      char const green1 = static_cast<char>(pixel_data.g[i] & MASK_BYTE);
      char const green2 = static_cast<char>((pixel_data.g[i] >> BITS_PER_BYTE) & MASK_BYTE);
      char const blue1  = static_cast<char>(pixel_data.b[i] & MASK_BYTE);
      char const blue2  = static_cast<char>((pixel_data.b[i] >> BITS_PER_BYTE) & MASK_BYTE);

      outfile.write(&red1, 1);
      outfile.write(&red2, 1);
      outfile.write(&green1, 1);
      outfile.write(&green2, 1);
      outfile.write(&blue1, 1);
      outfile.write(&blue2, 1);
    }
  } else {
    for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
      char const red   = static_cast<char>(pixel_data.r[i]);
      char const green = static_cast<char>(pixel_data.g[i]);
      char const blue  = static_cast<char>(pixel_data.b[i]);

      outfile.write(&red, 1);
      outfile.write(&green, 1);
      outfile.write(&blue, 1);
    }
  }
}