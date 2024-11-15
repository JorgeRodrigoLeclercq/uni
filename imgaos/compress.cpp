#include "compress.hpp"
#include <limits>

void write_color_table(std::ofstream &outfile, const std::vector<Pixel> &pixel_data,
                       const std::map<Pixel, int> &color_table) {
  if (auto const table_size = color_table.size(); table_size <= std::numeric_limits<uint8_t>::max()) {
    for (const auto &pixel : pixel_data) {
      auto const index = static_cast<uint8_t>(color_table.at(pixel));
      char const byte = static_cast<char>(index);
      outfile.write(&byte, 1);
    }
  } else if (table_size <= std::numeric_limits<uint16_t>::max()) {
    for (const auto &pixel : pixel_data) {
      auto const index = static_cast<uint16_t>(color_table.at(pixel));
      char const byte1 = static_cast<char>(index & 0xFF);
      char const byte2 = static_cast<char>((index >> 8) & 0xFF);
      outfile.write(&byte1, 1);
      outfile.write(&byte2, 1);
    }
  } else if (table_size <= std::numeric_limits<uint32_t>::max()) {
    for (const auto &pixel : pixel_data) {
      auto const index = static_cast<uint32_t>(color_table.at(pixel));
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

void compress(std::ofstream &cppm_outfile, ImageHeader &header, const std::vector<Pixel> &pixel_data) {
  constexpr uint8_t MAX_COLOR_VALUE8 = 255;

  std::map<Pixel, int> color_table;
  std::vector<Pixel> color_list;
  int color_index = 0;
  for (const auto &pixel : pixel_data) {
    if (!color_table.contains(pixel)) {
      color_table[pixel] = color_index++;
      color_list.push_back(pixel);
    }
  }

  header.magic_number = "C6";
  bool const is_16_bit = header.max_color > MAX_COLOR_VALUE8;

  write_info(cppm_outfile, header, color_list, is_16_bit);

  write_color_table(cppm_outfile, pixel_data, color_table);
}