#include "compress.hpp"

#include <map>

void write_cppm(std::ofstream& cppm_outfile, ImageHeader& header, SoA& pixel_data) {
    const uint8_t MAX_COLOR_VALUE8 = 255;

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

  // CPPM header
  cppm_outfile << "C6 " << header.dimensions.width << " " << header.dimensions.height << " " << header.max_color << " " << color_list.size() << "\n";

  bool const is_16_bit = header.max_color > MAX_COLOR_VALUE8;
  for (const auto &color_tuple : color_list) {
    uint8_t red = std::get<0>(color_tuple);
    uint8_t green = std::get<1>(color_tuple);
    uint8_t blue = std::get<2>(color_tuple);

    if (is_16_bit) {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      cppm_outfile.write(reinterpret_cast<const char*>(&red), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      cppm_outfile.write(reinterpret_cast<const char*>(&green), 2);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      cppm_outfile.write(reinterpret_cast<const char*>(&blue), 2);
    } else {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      cppm_outfile.write(reinterpret_cast<const char*>(&red), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      cppm_outfile.write(reinterpret_cast<const char*>(&green), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      cppm_outfile.write(reinterpret_cast<const char*>(&blue), 1);
    }
  }

  // Determinar el tamaño del índice basado en el tamaño de la tabla de colores
  auto table_size = color_list.size(); // `size_t` es más seguro aquí.

  if (table_size <= static_cast<size_t>(UINT8_MAX)) {
    for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
      auto color_tuple = std::make_tuple(pixel_data.r[i], pixel_data.g[i], pixel_data.b[i]);
      auto index = static_cast<uint8_t>(color_table[color_tuple]);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      cppm_outfile.write(reinterpret_cast<const char*>(&index), 1);
    }
  } else if (table_size <= static_cast<size_t>(UINT16_MAX)) {
    for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
      auto color_tuple = std::make_tuple(pixel_data.r[i], pixel_data.g[i], pixel_data.b[i]);
      auto index = static_cast<uint16_t>(color_table[color_tuple]);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      cppm_outfile.write(reinterpret_cast<const char*>(&index), 2);
    }
  } else if (table_size <= static_cast<size_t>(UINT32_MAX)) {
    for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
      auto color_tuple = std::make_tuple(pixel_data.r[i], pixel_data.g[i], pixel_data.b[i]);
      auto index = static_cast<uint32_t>(color_table[color_tuple]);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      cppm_outfile.write(reinterpret_cast<const char*>(&index), 4);
    }
  } else {
    std::cerr << "Error: Color table too large." << '\n';
    exit(1);
  }

}