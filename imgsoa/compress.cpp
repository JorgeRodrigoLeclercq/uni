#include "common/binaryio.hpp"
#include "info.hpp"
#include "compress.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdint>
#include <limits>
#include <algorithm>

// Escribir encabezado CPPM
void write_cppm_header(std::ofstream &outfile, const ImageHeader &header,
                       size_t const color_table_size) {
    outfile << "C6 ";
    outfile << header.dimensions.width << " ";
    outfile << header.dimensions.height << " ";
    outfile << header.max_color << " ";
    outfile << color_table_size << "\n";
}

// Escribir tabla de colores
void write_color_table(std::ofstream &outfile, SoA const &colors, int const max_color) {
    std::size_t const size = colors.r.size();
  for (std::size_t i = 0; i < size; ++i) {
    if (constexpr int MAX_8_BIT = 255; max_color <= MAX_8_BIT) {
      constexpr int CERO_FF = 0xFF;
      outfile.put(static_cast<char>(colors.r[i] & CERO_FF));
            outfile.put(static_cast<char>(colors.g[i] & CERO_FF));
            outfile.put(static_cast<char>(colors.b[i] & CERO_FF));
        } else {
            write_binary(outfile, colors.r[i]);
            write_binary(outfile, colors.g[i]);
            write_binary(outfile, colors.b[i]);
        }
    }
}

// Escribir datos de píxeles comprimidos
void write_compressed_pixel_data(std::ofstream &outfile, SoA const &pixels,
                                  std::map<std::size_t, uint32_t> const &color_table,
                                  size_t const index_size) {
    std::size_t const size = pixels.r.size();
    for (std::size_t i = 0; i < size; ++i) {
        uint32_t const index = color_table.at(i);
        if (index_size == 1) {
            auto const byte = static_cast<uint8_t>(index);
            outfile.put(static_cast<char>(byte));
        } else if (index_size == 2) {
            auto word = static_cast<uint16_t>(index);
            write_binary(outfile, word);
        } else if (index_size == 4) {
            write_binary(outfile, index);
        }
    }
}

// Comprimir imagen usando SoA
void compress(std::ofstream &outfile, const ImageHeader &header, SoA const &pixel_data) {
    // Crear una tabla de colores única
    std::map<std::size_t, uint32_t> color_table;
    SoA unique_colors;
    std::size_t const size = pixel_data.r.size();
    for (std::size_t i = 0; i < size; ++i) {
        auto iter = std::ranges::find_if(unique_colors.r,
            [&](uint16_t const &) {
                return unique_colors.is_equal(unique_colors.r.size(), i);
            });
        if (iter == unique_colors.r.end()) {
            if (unique_colors.r.size() > std::numeric_limits<uint32_t>::max()) {
                std::cerr << "Error: Too many unique colors for uint32_t indexing." << '\n';
                return;
            }
            unique_colors.r.push_back(pixel_data.r[i]);
            unique_colors.g.push_back(pixel_data.g[i]);
            unique_colors.b.push_back(pixel_data.b[i]);
            color_table[i] = static_cast<uint32_t>(unique_colors.r.size() - 1);
        }
    }
    // Determinar el tamaño de cada índice en bytes
    size_t const color_table_size = unique_colors.r.size();
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
    write_cppm_header(outfile, header, color_table_size);  // escribir encabezado
    write_color_table(outfile, unique_colors, header.max_color);  // escribir tabla de colores
    write_compressed_pixel_data(outfile, pixel_data, color_table, index_size);  // escribir datos de píxeles comprimidos
}