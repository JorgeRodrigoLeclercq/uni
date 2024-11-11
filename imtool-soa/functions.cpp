#include "functions.hpp"
#include <algorithm> // For std::clamp
#include <cmath>
#include <bits/algorithmfwd.h>
#include <fstream>
#include <iostream>

// Guardar la información del header de la imagen ppm en magic_number, width, height y max_color
void get_header(std::ifstream &infile, ImageHeader &header) {
  const int MAX_IGNORE_CHARS = 256;
    // Leer el header
    infile >> header.magic_number >> header.dimensions.width >> header.dimensions.height >> header.max_color;

    if (header.magic_number != "P6") {
        std::cerr << "Error: This program only supports P6 (binary) format" << '\n';
        exit(1);
    }

    // Saltar espacios blancos
    infile.ignore(MAX_IGNORE_CHARS, '\n');

    // Log el header
    std::cout << "Magic Number: " << header.magic_number << '\n';
    std::cout << "Width: " << header.dimensions.width << '\n';
    std::cout << "Height: " << header.dimensions.height << '\n';
    std::cout << "Max Color: " << header.max_color << '\n';
}

void get_pixels(std::ifstream &infile, SoA &pixel_data, unsigned long long pixel_count, bool is_16_bit) {
    const int BIT_SHIFT = 8;
    const int FIRST_PIXELS = 10;

  if (is_16_bit) {
    // max_color > 255, por lo tanto, bits por píxel = 2, en little-endian
    for (unsigned long long int i = 0; i < pixel_count; ++i) {
      uint8_t red1 = 0;
      uint8_t red2 = 0;
      uint8_t green1 = 0;
      uint8_t green2 = 0;
      uint8_t blue1 = 0;
      uint8_t blue2 = 0;

      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char*>(&red1), 1);  // Leer primer byte de rojo
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char*>(&red2), 1);  // Leer segundo byte de rojo

      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char*>(&green1), 1);  // Leer primer byte de verde
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char*>(&green2), 1);  // Leer segundo byte de verde

      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char*>(&blue1), 1);  // Leer primer byte de azul
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char*>(&blue2), 1);  // Leer segundo byte de azul

      // Little-endian: combinar los dos bytes para cada componente de color
      pixel_data.r[i] = static_cast<uint16_t>(red1 | (red2 << BIT_SHIFT));
      pixel_data.g[i] = static_cast<uint16_t>(green1 | (green2 << BIT_SHIFT));
      pixel_data.b[i] = static_cast<uint16_t>(blue1 | (blue2 << BIT_SHIFT));
    }
  } else {
    // max_color <= 255, por lo tanto, bits por píxel = 1
    for (unsigned long long i = 0; i < pixel_count; ++i) {
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char*>(&pixel_data.r[i]), 1);  // Leer un byte de rojo

      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char*>(&pixel_data.g[i]), 1);  // Leer un byte de verde

      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      infile.read(reinterpret_cast<char*>(&pixel_data.b[i]), 1);  // Leer un byte de azul
    }
  }

  // Log de los primeros píxeles
  std::cout << "Primeros píxeles (valores RGB):" << '\n';
  for (unsigned long long int i = 0; i < std::min(static_cast<unsigned long long int>(FIRST_PIXELS), pixel_count); ++i) {
    std::cout << "Píxel " << i << ": "
              << "R = " << static_cast<int>(pixel_data.r[i]) << ", "
              << "G = " << static_cast<int>(pixel_data.g[i]) << ", "
              << "B = " << static_cast<int>(pixel_data.b[i]) << '\n';
  }
}

void write_info(std::ofstream& outfile, ImageHeader& header, SoA& pixel_data, bool is_16_bit) {
    const uint16_t MASK_BYTE = 0xFF;
    const int BITS_PER_BYTE = 8;

  outfile << header.magic_number << "\n";
  outfile << header.dimensions.width << " " << header.dimensions.height << "\n";
  outfile << header.max_color << "\n";

  if (is_16_bit) {
    for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
      uint8_t red1 = pixel_data.r[i] & MASK_BYTE;
      uint8_t red2 = (pixel_data.r[i] >> BITS_PER_BYTE) & MASK_BYTE;
      uint8_t green1 = pixel_data.g[i] & MASK_BYTE;
      uint8_t green2 = (pixel_data.g[i] >> BITS_PER_BYTE) & MASK_BYTE;
      uint8_t blue1 = pixel_data.b[i] & MASK_BYTE;
      uint8_t blue2 = (pixel_data.b[i] >> BITS_PER_BYTE) & MASK_BYTE;

      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<const char*>(&red1), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<const char*>(&red2), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<const char*>(&green1), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<const char*>(&green2), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<const char*>(&blue1), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<const char*>(&blue2), 1);
    }
  } else {
    // Para imágenes de 8 bits por componente
    for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
      // Escribir un byte de cada componente de color
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<const char*>(&pixel_data.r[i]), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<const char*>(&pixel_data.g[i]), 1);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outfile.write(reinterpret_cast<const char*>(&pixel_data.b[i]), 1);
    }
  }
}

void write_cppm(std::ofstream& cppm_outfile, ImageHeader& header, SoA& pixel_data) {
    const uint8_t MAX_COLOR_VALUE8 = 255;

  // Mapear cada índice a su color
  std::map<std::tuple<uint8_t, uint8_t, uint8_t>, int> color_table;
  std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> color_list;

  int color_index = 0;
  for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
    auto color_tuple = std::make_tuple(pixel_data.r[i], pixel_data.g[i], pixel_data.b[i]);

    if (color_table.find(color_tuple) == color_table.end()) {
      color_table[color_tuple] = color_index++;
      color_list.push_back(color_tuple);
    }
  }

  // CPPM header
  cppm_outfile << "C6 " << header.dimensions.width << " " << header.dimensions.height << " " << header.max_color << " " << color_list.size() << "\n";

  bool is_16_bit = header.max_color > MAX_COLOR_VALUE8;
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

constexpr int MAX_COLOR_8BIT = 255;

void maxlevel(int new_maxlevel, bool& is_16_bit, SoA& pixel_data, ImageHeader& header) {
  // Determinar si la salida será de 8 o 16 bits
  is_16_bit = new_maxlevel > MAX_COLOR_8BIT;

  // Escalar los componentes de color sin redondeo para cada canal
  for (size_t i = 0; i < pixel_data.r.size(); ++i) {
    uint16_t r_scaled = static_cast<uint16_t>(
        std::clamp(static_cast<int>(static_cast<float>(pixel_data.r[i]) * static_cast<float>(new_maxlevel) / static_cast<float>(header.max_color)),
                   0, new_maxlevel));
    uint16_t g_scaled = static_cast<uint16_t>(
        std::clamp(static_cast<int>(static_cast<float>(pixel_data.g[i]) * static_cast<float>(new_maxlevel) / static_cast<float>(header.max_color)),
                   0, new_maxlevel));
    uint16_t b_scaled = static_cast<uint16_t>(
        std::clamp(static_cast<int>(static_cast<float>(pixel_data.b[i]) * static_cast<float>(new_maxlevel) / static_cast<float>(header.max_color)),
                   0, new_maxlevel));

    // Asignar los valores escalados
    pixel_data.r[i] = r_scaled;
    pixel_data.g[i] = g_scaled;
    pixel_data.b[i] = b_scaled;
  }

  // Actualizar max_color al nuevo nivel máximo
  header.max_color = new_maxlevel;
}


SoA ReSize( ImageDimensions & original_dimension,  SoA & pixel_Data,
                     const ImageDimensions & new_dimension) {
  const auto pixel_count = static_cast<size_t>( std::abs(new_dimension.width * new_dimension.height));

  // Structure of Arrays
  SoA new_pixel_data;
  new_pixel_data.r.resize(pixel_count);
  new_pixel_data.g.resize(pixel_count);
  new_pixel_data.b.resize(pixel_count);
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
      std::vector<uint16_t> new_data = interpolacion_colores(pixel_Data, coordenadas, i , original_dimension);
      new_pixel_data.r[static_cast<unsigned long int >(j + (i * new_dimension.width))] = new_data[0];
      new_pixel_data.g[static_cast<unsigned long int> (j  + (i * new_dimension.width))] = new_data[1];
      new_pixel_data.b[static_cast<unsigned long int> (j  + (i * new_dimension.width))] = new_data[2];
    }
  }
  return new_pixel_data;


}

std::vector<uint16_t> interpolacion_colores ( const SoA &pixel_Data, const std::vector<double> &coordenadas , const int width_counter , const ImageDimensions &original_dimension ) {
              std::vector<uint16_t> new_colors ;
              std::vector<double> first_point = {0,0,0};
              std::vector<double> second_point = {0,0,0};
              double color_c1 = 0.0;
              double color_c2 = 0.0;

                first_point = {coordenadas[1], coordenadas[4], static_cast<double>(pixel_Data.r[static_cast<unsigned long long int>(
                            static_cast<long>(coordenadas[1] + coordenadas[4]) *
                            original_dimension.width)])};
                second_point = {coordenadas[2], coordenadas[4], static_cast<double>(pixel_Data.r[static_cast<unsigned long long int>(
                                          (coordenadas[2] + coordenadas[4]) *
                                          original_dimension.width)])};
                color_c1 = interpolacion(first_point, second_point, width_counter);
                first_point = {coordenadas[1], coordenadas[ 4 + 1 ],static_cast<double>(pixel_Data.r[static_cast<unsigned long long int>(
                                              (coordenadas[1] + coordenadas[4 + 1]) *
                                              original_dimension.width)])};
                second_point = {coordenadas[2], coordenadas[4 + 1], static_cast<double>(pixel_Data.r[static_cast<unsigned long long int>(
                                               (coordenadas[2] + coordenadas[4 + 1]) *
                                               original_dimension.width)])};
                color_c2 = interpolacion(first_point, second_point, width_counter);
                first_point = {coordenadas[0], coordenadas[4], color_c1};
                second_point = {coordenadas[0],coordenadas[4 + 1], color_c2};

                new_colors.push_back( static_cast<uint16_t>(interpolacion(first_point, second_point, width_counter)));



               return new_colors;

}

double interpolacion( const std::vector<double>  &first_point ,const std::vector<double> &second_point , const  int y_value) {
      //Formula for getting the z ( color) value of the interpolation of two thredimensional points
      return ( first_point[2] + (( second_point[2] - first_point[2]) * ((y_value - first_point[1]) / ( second_point[1] - first_point[0]))));

    }

