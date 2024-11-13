#include "compress.hpp"

#include <limits>

void write_color_table(std::ofstream &outfile, SoA& pixel_data,
                       std::map<std::tuple<uint8_t, uint8_t, uint8_t>, int> const &color_table) {
  if (auto const table_size = color_table.size(); table_size <= std::numeric_limits<uint8_t>::max()) {
    for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
      auto color_tuple = std::make_tuple(pixel_data.r[i], pixel_data.g[i], pixel_data.b[i]);
      uint8_t const index = static_cast<uint8_t>(color_table.at(color_tuple));
      char const byte = static_cast<char>(index);
      outfile.write(&byte, 1);
    }
  } else if (table_size <= std::numeric_limits<uint16_t>::max()) {
    for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
      auto color_tuple = std::make_tuple(pixel_data.r[i], pixel_data.g[i], pixel_data.b[i]);
      uint16_t const index = static_cast<uint16_t>(color_table.at(color_tuple));
      char const byte1 = static_cast<char>(index & 0xFF);
      char const byte2 = static_cast<char>((index >> 8) & 0xFF);
      outfile.write(&byte1, 1);
      outfile.write(&byte2, 1);
    }
  } else if (table_size <= std::numeric_limits<uint32_t>::max()) {
    for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
      auto color_tuple = std::make_tuple(pixel_data.r[i], pixel_data.g[i], pixel_data.b[i]);
      uint32_t const index = static_cast<uint32_t>(color_table.at(color_tuple));
      char const byte1 = static_cast<char>(index & 0xFF);
      char const byte2 = static_cast<char>((index >> 8) & 0xFF);
      char const byte3 = static_cast<char>((index >> 16) & 0xFF);
      char const byte4 = static_cast<char>((index >> 24) & 0xFF);
      outfile.write(&byte1, 1);
      outfile.write(&byte2, 1);
      outfile.write(&byte3, 1);
      outfile.write(&byte4, 1);
    }
  } else {
    std::cerr << "Error: Color table too large." << '\n';
    exit(1);
  }
}

void compress(std::ofstream& cppm_outfile, ImageHeader & header, SoA& pixel_data) {
  constexpr uint8_t MAX_COLOR_VALUE8 = 255;

  // Mapear cada índice a su color
  std::map<std::tuple<uint8_t, uint8_t, uint8_t>, int> color_table;
  std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> color_list;

  int color_index = 0;
  for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
    if (auto color_tuple = std::make_tuple(pixel_data.r[i], pixel_data.g[i], pixel_data.b[i]);
        !color_table.contains(color_tuple)) {
      color_table[color_tuple] = color_index++;
      color_list.emplace_back(color_tuple);
    }
  }

  header.magic_number = "C6";
  bool const is_16_bit = header.max_color > MAX_COLOR_VALUE8;

  write_info(cppm_outfile, header, pixel_data, is_16_bit);

  write_color_table(cppm_outfile, pixel_data, color_table);
}