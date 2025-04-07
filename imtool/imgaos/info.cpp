#include "info.hpp"
#include "common/binaryio.hpp"
#include <fstream>

// Guardar los pixeles de la imagen ppm en una estructura AoS
void get_pixels(std::ifstream &infile, std::vector<Pixel> &pixel_data,
                unsigned long long const pixel_count, bool const is_16_bit) {
  if (is_16_bit) {
    for (unsigned long long int i = 0; i < pixel_count * 3; ++i) {
      pixel_data[i].channels.red = read_binary<uint16_t>(infile);
      pixel_data[i].channels.green = read_binary<uint16_t>(infile);
      pixel_data[i].channels.blue = read_binary<uint16_t>(infile);
    }
  } else {
    for (unsigned long long i = 0; i < pixel_count ; ++i) {
      pixel_data[i].channels.red   = read_binary<uint8_t>(infile);
      pixel_data[i].channels.green = read_binary<uint8_t>(infile);
      pixel_data[i].channels.blue  = read_binary<uint8_t>(infile);
    }
  }
}

// Escribir la información de la imagen en el archivo de salida
void write_info(std::ofstream &outfile, const ImageHeader &header, const std::vector<Pixel> &pixel_data, bool const is_16_bit) {
  outfile << header.magic_number << "\n";
  outfile << header.dimensions.width << " " << header.dimensions.height << "\n";
  outfile << header.max_color << "\n";
  if (is_16_bit) {
    for (auto const &pixel : pixel_data) {
      write_binary(outfile, pixel.channels.red);
      write_binary(outfile, pixel.channels.green);
      write_binary(outfile, pixel.channels.blue);
    }
  } else {
    for (auto const &pixel : pixel_data) {
      write_binary(outfile, static_cast<uint8_t>(pixel.channels.red));
      write_binary(outfile, static_cast<uint8_t>(pixel.channels.green));
      write_binary(outfile, static_cast<uint8_t>(pixel.channels.blue));
    }
  }
}

// Imprimir información por pantalla
void info(const ImageHeader& header, const std::vector<Pixel>& pixel_data) {
  // Header
  std::cout << "PPM Image Information:" << '\n';
  std::cout << "Magic Number: " << header.magic_number << '\n';
  std::cout << "Dimensions: " << header.dimensions.width << " x " << header.dimensions.height << '\n';
  std::cout << "Max Color Value: " << header.max_color << '\n';

  // Pixel data
  std::cout << "\nPixel Data:" << '\n';
  for (size_t i = 0; i < pixel_data.size(); ++i) {
    std::cout << "Pixel " << i + 1 << ": ";
    std::cout << "R=" << static_cast<int>(pixel_data[i].channels.red) << ", "
              << "G=" << static_cast<int>(pixel_data[i].channels.green) << ", "
              << "B=" << static_cast<int>(pixel_data[i].channels.blue) << '\n';
  }
}