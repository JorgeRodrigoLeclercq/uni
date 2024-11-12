#include "compress.hpp"
#include <limits>

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