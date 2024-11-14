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

// Imprimir la información de una imagen PPM usando SoA
void info(const ImageHeader& header, const SoA& pixel_data) {
  // Print header information
  std::cout << "PPM Image Information:" << std::endl;
  std::cout << "Magic Number: " << header.magic_number << std::endl;
  std::cout << "Dimensions: " << header.dimensions.width << " x " << header.dimensions.height << std::endl;
  std::cout << "Max Color Value: " << header.max_color << std::endl;

  // Print pixel data
  std::cout << "\nPixel Data:" << std::endl;
  size_t pixel_count = pixel_data.r.size();
  for (size_t i = 0; i < pixel_count; ++i) {
    std::cout << "Pixel " << i + 1 << ": ";

    if (header.max_color > 255) {
      // For 16-bit images, print directly as uint16_t
      std::cout << "R=" << pixel_data.r[i] << ", "
                << "G=" << pixel_data.g[i] << ", "
                << "B=" << pixel_data.b[i] << std::endl;
    } else {
      // For 8-bit images, cast to uint8_t
      std::cout << "R=" << static_cast<uint8_t>(pixel_data.r[i]) << ", "
                << "G=" << static_cast<uint8_t>(pixel_data.g[i]) << ", "
                << "B=" << static_cast<uint8_t>(pixel_data.b[i]) << std::endl;
    }
  }
}
