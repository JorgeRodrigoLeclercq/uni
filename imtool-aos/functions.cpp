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
/*
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
*/
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


void maxlevel(int new_maxlevel, bool& is_16_bit, gsl::span<Pixel> &pixel_data, ImageHeader &header) {
    constexpr int MAX_COLOR_8BIT = 255;
  // Determinar si la salida será de 8 o 16 bits
  is_16_bit = new_maxlevel > MAX_COLOR_8BIT;

  for (auto& pixel : pixel_data) {
    // Escalar cada componente de color
    uint16_t r_scaled = static_cast<uint16_t>(
        std::clamp(static_cast<int>(static_cast<float>(pixel.channels.red) * static_cast<float>(new_maxlevel) / static_cast<float>(header.max_color)),
              0, new_maxlevel));
    uint16_t g_scaled = static_cast<uint16_t>(
        std::clamp(static_cast<int>(static_cast<float>(pixel.channels.green) * static_cast<float>(new_maxlevel) / static_cast<float>(header.max_color)),
              0, new_maxlevel));
    uint16_t b_scaled = static_cast<uint16_t>(
        std::clamp(static_cast<int>(static_cast<float>(pixel.channels.blue) * static_cast<float>(new_maxlevel) / static_cast<float>(header.max_color)),
              0, new_maxlevel));

    // Asignar valores escalados en 8 o 16 bits
    pixel.channels.red = is_16_bit ? r_scaled : static_cast<uint8_t>(r_scaled);
    pixel.channels.green = is_16_bit ? g_scaled : static_cast<uint8_t>(g_scaled);
    pixel.channels.blue = is_16_bit ? b_scaled : static_cast<uint8_t>(b_scaled);
  }

  // Actualizar max_color al nuevo nivel máximo
  header.max_color = new_maxlevel;
}

double interpolacion(const std::vector<double>  &first_point , const std::vector<double> & second_point , const int y_value) {
   //Formula for getting the z ( color) value of the interpolation of two thredimensional points
      return ( first_point[2] + (( second_point[2] - first_point[2]) * ((y_value - first_point[1]) / ( second_point[1] - first_point[0]))));

}

Pixel interpolacion_colores ( const std::vector<Pixel> &pixel_Data, std::vector<double> &coordenadas , const int width_counter , const ImageDimensions &original_dimension ) {

  Pixel pixel;
              std::vector<double> first_point = {coordenadas[1], coordenadas[4], (double)pixel_Data[static_cast<unsigned long long int>(
                            static_cast<long>(coordenadas[1] + coordenadas[4]) *
                            original_dimension.width)].channels.red};
              std::vector<double> second_point = {coordenadas[2], coordenadas[4], (double)pixel_Data[static_cast<unsigned long long int>(
                                        static_cast<long>(coordenadas[2] + coordenadas[4]) *
                                        original_dimension.width)].channels.red};
              double color_c1 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[1], coordenadas[4+1],static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                            static_cast<long>(coordenadas[1] + coordenadas[4+1]) *
                                            original_dimension.width)].channels.red)};
              second_point = {coordenadas[2], coordenadas[4 +1], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                             static_cast<long>(coordenadas[2] + coordenadas[4 +1]) *
                                             original_dimension.width)].channels.red)};
              double color_c2 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[0], coordenadas[4], color_c1};
              second_point = {coordenadas[0],coordenadas[4 +1], color_c2};
              pixel.channels.red      = static_cast<uint16_t>(interpolacion(first_point, second_point, width_counter));
              // --g
              first_point = {coordenadas[1], coordenadas[4], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                            static_cast<long>(coordenadas[1] + coordenadas[4]) *
                                            original_dimension.width)].channels.green)};
              second_point = {coordenadas[2], coordenadas[4], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                             static_cast<long>(coordenadas[2] + coordenadas[4]) *
                                             original_dimension.width)].channels.green)};
              color_c1 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[1], coordenadas[4 +1 ], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                            static_cast<long>(coordenadas[1] + coordenadas[4 + 1]) *
                                            original_dimension.width)].channels.green)};
              second_point = {coordenadas[2], coordenadas[4 + 1], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                             static_cast<long>(coordenadas[2] + coordenadas[4 +1]) *
                                             original_dimension.width)].channels.green)};
              color_c2 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[0], coordenadas[4], color_c1};
              second_point = {coordenadas[0], coordenadas[4 +1], color_c2};
              pixel.channels.green = static_cast<uint16_t>(interpolacion(first_point, second_point, width_counter));
              // --b
              first_point = {coordenadas[1], coordenadas[4], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                            static_cast<long>(coordenadas[1] + coordenadas[4]) *
                                            original_dimension.width)].channels.blue)};
              second_point = {coordenadas[2], coordenadas[4], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                             static_cast<long>(coordenadas[2] + coordenadas[4]) *
                                             original_dimension.width)].channels.blue)};
              color_c1 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[1], coordenadas[4+1], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                            static_cast<long>(coordenadas[1] + coordenadas[4+ 1]) *
                                            original_dimension.width)].channels.blue)};
              second_point = {coordenadas[2], coordenadas[4+1], static_cast<double>(pixel_Data[static_cast<unsigned long long int>(
                                             static_cast<long>(coordenadas[2] + coordenadas[4+ 1]) *
                                             original_dimension.width)].channels.blue)};
              color_c2 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[0], coordenadas[4], color_c1};
              second_point = {coordenadas[0], coordenadas[4+1], color_c2};
              pixel.channels.blue = static_cast<uint16_t>(interpolacion(first_point, second_point, width_counter));

  return pixel;

}

  void DimensionChange(const ImageDimensions& original_dimension, const  std::vector<Pixel> &pixel_Data , const ImageDimensions& new_dimension) {
      std::vector<Pixel> new_pixel_data(static_cast<std::size_t>(new_dimension.width * new_dimension.height));
      Pixel pixel; pixel.channels.red = 0; pixel.channels.green = 0; pixel.channels.blue = 0;
      double new_x =  0.0;
      double x_floor = 0.0;
      double x_ceil = 0.0;
      double new_y = 0.0;
      double y_floor = 0.0;
      double y_ceil = 0.0;
      std::vector<double> coordenadas = {new_x, x_floor, x_ceil, new_y , y_floor, y_ceil};

      for ( int i = 0; i < new_dimension.height; i++ ) {
            for ( int j = 0; j < new_dimension.width; j++ ) {
              new_x = static_cast<double>( j * original_dimension.width) / static_cast<double>(new_dimension.width);
              x_floor = std::floor(new_x);
              x_ceil = std::ceil(new_x);
              new_y =  static_cast<double>( i * original_dimension.height) / static_cast<double>(new_dimension.height);
              y_floor = std::floor(new_y);
              y_ceil = std::ceil(new_y);

              coordenadas = {new_x, x_floor, x_ceil, new_y , y_floor, y_ceil};
              //We write the new data in our new image
              new_pixel_data[static_cast<unsigned long int>(j + (i * new_dimension.width))] = interpolacion_colores(pixel_Data, coordenadas, i , original_dimension);
            }
      }
}
