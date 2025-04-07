#include "compress.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdint>
#include <limits>
#include "info.hpp"
#include "common/binaryio.hpp"

// Escribir encabezado CPPM
void write_cppm_header(std::ofstream &outfile, const ImageHeader &header, size_t color_table_size) {
    outfile << "C6 ";
    outfile << header.dimensions.width << " ";
    outfile << header.dimensions.height << " ";
    outfile << header.max_color << " ";
    outfile << color_table_size << "\n";
}

// Escribir tabla de colores
void write_color_table(std::ofstream &outfile, const std::vector<Pixel> &unique_colors,
                       int const max_color) {
  for (const auto &color : unique_colors) {
    if (constexpr int MAX_8_BIT = 255; max_color <= MAX_8_BIT) {
      outfile.put(static_cast<char>(color.channels.red));
      outfile.put(static_cast<char>(color.channels.green));
      outfile.put(static_cast<char>(color.channels.blue));
    } else {
      write_binary(outfile, color.channels.red);
      write_binary(outfile, color.channels.green);
      write_binary(outfile, color.channels.blue);
    }
  }
}

// Escribir datos de píxeles comprimidos
void write_compressed_pixel_data(std::ofstream &outfile, const std::vector<Pixel> &pixel_data,
                                  const std::map<Pixel, uint32_t> &color_table, size_t index_size) {
  for (const auto &pixel : pixel_data) {
    uint32_t const index = color_table.at(pixel);
    if (index_size == 1) {
      outfile.put(static_cast<char>(index));
    } else if (index_size == 2) {
      write_binary(outfile, static_cast<uint16_t>(index));
    } else if (index_size == 4) {
      write_binary(outfile, index);
    }
  }
}

void compress( std::ofstream &outfile, const ImageHeader &header, const std::vector<Pixel> &pixel_data) {
  // Crear una tabla de colores única
  std::map<Pixel, uint32_t> color_table;
  std::vector<Pixel> unique_colors;
  for (const auto &pixel : pixel_data) {
    if (!color_table.contains(pixel)) {
      if (unique_colors.size() > std::numeric_limits<uint32_t>::max()) {
        std::cerr << "Error: Too many unique colors for uint32_t indexing." << '\n';
        return;
      }
      color_table[pixel] = static_cast<uint32_t>(unique_colors.size());
      unique_colors.push_back(pixel);
    }
  }
  // Determinar el tamaño de cada índice en bytes
  size_t const color_table_size = unique_colors.size();
  size_t index_size = 0;
  if (constexpr int MAX_8_BIT = 256; color_table_size <= MAX_8_BIT) {
    index_size = 1;
  } else if (constexpr int MAX_16_BIT = 65536; color_table_size <= MAX_16_BIT) {
    index_size = 2;
  } else if (constexpr long MAX_32_BIT = 4294967296;
             color_table_size <= MAX_32_BIT) { // Máximo para un uint32_t
    index_size = 4;
             } else {
               std::cerr << "Error: Color table size exceeds supported limits." << '\n';
               return;
             }
  // Escribir encabezado
  write_cppm_header(outfile, header, color_table_size);
  // Escribir tabla de colores
  write_color_table(outfile, unique_colors, header.max_color);
  // Escribir datos de píxeles comprimidos
  write_compressed_pixel_data(outfile, pixel_data, color_table, index_size);
}