#include "functions.hpp"
#include <iostream>
#include <fstream>

// Guardar los pixeles de la imagen ppm en una estructura AoS
void get_pixels(std::ifstream & infile, std::vector<Pixel> & pixel_data,
                unsigned long long const pixel_count, bool const is_16_bit) {
  if (is_16_bit) {
    // max_color > 255 t por ende, bits en pixel == 2, en little-endian
    for (unsigned long long int i = 0; i < pixel_count; ++i) {
      constexpr int BIT_SHIFT = 8;
      uint8_t red1            = 0;
      uint8_t red2            = 0;
      uint8_t green1          = 0;
      uint8_t green2          = 0;
      uint8_t blue1           = 0;
      uint8_t blue2           = 0;
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char *>(&red1), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char *>(&red2), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char *>(&green1), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char *>(&green2), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char *>(&blue1), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char *>(&blue2), 1);

      // Little-endian
      pixel_data[i].channels.red   = static_cast<uint16_t>(red1 | (red2 << BIT_SHIFT));
      pixel_data[i].channels.green = static_cast<uint16_t>(green1 | (green2 << BIT_SHIFT));
      pixel_data[i].channels.blue  = static_cast<uint16_t>(blue1 | (blue2 << BIT_SHIFT));
    }
  } else {
    // max_color < 255 y por ende, bits en pixel == 1
    for (unsigned long long i = 0; i < pixel_count; ++i) {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char *>(&pixel_data[i].channels.red), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char *>(&pixel_data[i].channels.green), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char *>(&pixel_data[i].channels.blue), 1);
    }
  }
}

// Ecribir la información de la imagen en el archivo de salida
void write_info(std::ofstream &outfile, const ImageHeader &header, const std::vector<Pixel> &pixel_data, bool is_16_bit) {
  outfile << header.magic_number << "\n";
  outfile << header.dimensions.width << " " << header.dimensions.height << "\n";
  outfile << header.max_color << "\n";

  if (is_16_bit) {
    for (const auto &pixel : pixel_data) {
      constexpr int BITS_PER_BYTE      = 8;
      constexpr uint16_t MASK_BYTE = 0xFF;
      uint8_t red1 = pixel.channels.red & MASK_BYTE;
      uint8_t red2 = (pixel.channels.red >> BITS_PER_BYTE) & MASK_BYTE;
      uint8_t green1 = pixel.channels.green & MASK_BYTE;
      uint8_t green2 = (pixel.channels.green >> BITS_PER_BYTE) & MASK_BYTE;
      uint8_t blue1 = pixel.channels.blue & MASK_BYTE;
      uint8_t blue2 = (pixel.channels.blue >> BITS_PER_BYTE) & MASK_BYTE;
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<char const *>(&red1), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<char const *>(&red2), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<char const *>(&green1), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<char const *>(&green2), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<char const *>(&blue1), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<char const *>(&blue2), 1);
    }
  } else {
    for (auto const & pixel : pixel_data) {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<char const *>(&pixel.channels.red), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<char const *>(&pixel.channels.green), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<char const *>(&pixel.channels.blue), 1);
    }
  }
}


