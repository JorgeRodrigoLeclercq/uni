#include "functions.hpp"
#include <cmath>
#include <bits/algorithmfwd.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>

// Guardar la información del header de la imagen ppm en magic_number, width, height y max_color
void get_header(std::ifstream &infile, ImageHeader &header) {
  constexpr int MAX_IGNORE_CHARS = 256;
  // Leer el header
  infile >> header.magic_number >> header.dimensions.width >> header.dimensions.height >>
      header.max_color;

  if (header.magic_number != "P6") {
    std::cerr << "Error: This program only supports P6 (binary) format" << '\n';
    exit(1);
  }

  // Saltar espacios blancos
  infile.ignore(MAX_IGNORE_CHARS, '\n');
}

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

// Function to write the color table indices based on the color map
void write_color_table(std::ofstream &outfile, const std::vector<Pixel> &pixel_data,
                       const std::map<Pixel, int> &color_table) {
  if (auto const table_size = color_table.size(); table_size <= std::numeric_limits<uint8_t>::max()) {
    for (const auto &pixel : pixel_data) {
      auto index = static_cast<uint8_t>(color_table.at(pixel));
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<const char*>(&index), 1);
    }
  } else if (table_size <= std::numeric_limits<uint16_t>::max()) {
    for (const auto &pixel : pixel_data) {
      auto index = static_cast<uint16_t>(color_table.at(pixel));
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<const char*>(&index), 2);
    }
  } else if (table_size <= std::numeric_limits<uint32_t>::max()) {
    for (const auto &pixel : pixel_data) {
      auto index = static_cast<uint32_t>(color_table.at(pixel));
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<const char*>(&index), 4);
    }
  } else {
    std::cerr << "Error: Color table too large." << '\n';
    exit(1);
  }
}

void write_cppm(std::ofstream &cppm_outfile, const ImageHeader &header, const std::vector<Pixel> &pixel_data) {
  constexpr uint8_t MAX_COLOR_VALUE8 = 255;

  // Mapear cada índice a su color
  std::map<Pixel, int> color_table;
  std::vector<Pixel> color_list;

  int color_index = 0;
  for (const auto &pixel : pixel_data) {
    if (!color_table.contains(pixel)  ) {
      color_table[pixel] = color_index++;
      color_list.push_back(pixel);
    }
  }

  // CPPM header
  cppm_outfile << "C6 " << header.dimensions.width << " " << header.dimensions.height << " " << header.max_color << " " << color_list.size() << "\n";

  bool const is_16_bit = header.max_color > MAX_COLOR_VALUE8;
  for (const auto &color : color_list) {
    if (is_16_bit) {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      cppm_outfile.write(reinterpret_cast<const char*>(&color.channels.red), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      cppm_outfile.write(reinterpret_cast<const char*>(&color.channels.green), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      cppm_outfile.write(reinterpret_cast<const char*>(&color.channels.blue), 2);
    } else {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      cppm_outfile.write(reinterpret_cast<const char*>(&color.channels.red), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      cppm_outfile.write(reinterpret_cast<const char*>(&color.channels.green), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      cppm_outfile.write(reinterpret_cast<const char*>(&color.channels.blue), 1);
    }
  }

  write_color_table(cppm_outfile, pixel_data, color_table);
}


