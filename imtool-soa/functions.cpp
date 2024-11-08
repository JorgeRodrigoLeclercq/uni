#include "functions.hpp"
#include <algorithm> // For std::clamp
#include <cmath>
#include <bits/algorithmfwd.h>

// Guardar la información del header de la imagen ppm en magic_number, width, height y max_color
void get_header(std::ifstream &infile, std::string &magic_number, int &width, int &height, int &max_color) {
    // Leer el header
    infile >> magic_number >> width >> height >> max_color;

    if (magic_number != "P6") {
        std::cerr << "Error: This program only supports P6 (binary) format" << '\n';
        exit(1);
    }

    // Saltar espacios blancos
    infile.ignore(256, '\n');

    // Log el header
    std::cout << "Magic Number: " << magic_number << '\n';
    std::cout << "Width: " << width << '\n';
    std::cout << "Height: " << height << '\n';
    std::cout << "Max Color: " << max_color << '\n';
}

// Guardar los pixeles de la imagen ppm en una estructura SoA
void get_pixels(std::ifstream &infile, SoA &pixel_data, int pixel_count, bool is_16_bit) {
    if (is_16_bit) {
        // max_color > 255 t por ende, bits en pixel == 2, en little-endian
        for (int i = 0; i < pixel_count; ++i) {
            uint8_t r1;
            uint8_t r2;
            uint8_t g1;
            uint8_t g2;
            uint8_t b1;
            uint8_t b2;
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&r1), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&r2), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&g1), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&g2), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&b1), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&b2), 1);

            // Little-endian
            pixel_data.r[i] = r1 | (r2 << 8);
            pixel_data.g[i] = g1 | (g2 << 8);
            pixel_data.b[i] = b1 | (b2 << 8);
        }
    } else {
        // max_color > 255 y por ende, bits en pixel == 1
        for (int i = 0; i < pixel_count; ++i) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&pixel_data.r[i]), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&pixel_data.g[i]), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&pixel_data.b[i]), 1);
        }
    }

    // Log pixels
    std::cout << "First few pixels (RGB values):" << '\n';
    for (int i = 0; i < std::min(10, pixel_count); ++i) {
        std::cout << "Pixel " << i << ": "
                  << "R = " << static_cast<int>(pixel_data.r[i]) << ", "
                  << "G = " << static_cast<int>(pixel_data.g[i]) << ", "
                  << "B = " << static_cast<int>(pixel_data.b[i]) << '\n';
    }
}

void write_info(std::ofstream &outfile, const std::string &magic_number, int width, int height, int max_color, SoA &pixel_data, bool is_16_bit) {
    outfile << magic_number << "\n";
    outfile << width << " " << height << "\n";
    outfile << max_color << "\n";

    if (is_16_bit) {
        for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
            uint8_t r1 = pixel_data.r[i] & 0xFF;
            uint8_t r2 = (pixel_data.r[i] >> 8) & 0xFF;
            uint8_t g1 = pixel_data.g[i] & 0xFF;
            uint8_t g2 = (pixel_data.g[i] >> 8) & 0xFF;
            uint8_t b1 = pixel_data.b[i] & 0xFF;
            uint8_t b2 = (pixel_data.b[i] >> 8) & 0xFF;
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            outfile.write(reinterpret_cast<const char*>(&r1), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            outfile.write(reinterpret_cast<const char*>(&r2), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            outfile.write(reinterpret_cast<const char*>(&g1), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            outfile.write(reinterpret_cast<const char*>(&g2), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            outfile.write(reinterpret_cast<const char*>(&b1), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            outfile.write(reinterpret_cast<const char*>(&b2), 1);
        }
    } else {
        for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            outfile.write(reinterpret_cast<const char*>(&pixel_data.r[i]), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            outfile.write(reinterpret_cast<const char*>(&pixel_data.g[i]), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            outfile.write(reinterpret_cast<const char*>(&pixel_data.b[i]), 1);
        }
    }
}

//Ecribir la información de la imagen en el archivo de salida

void write_cppm(std::ofstream &cppm_outfile, SoA &pixel_data, int width, int height, int max_color) {
    // Mapear cada pixel a un índice
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
    cppm_outfile << "P6 " << width << " " << height << " " << max_color << " " << color_list.size() << "\n";

    bool is_16_bit = max_color > 255;
    for (const auto &color_tuple : color_list) {
        uint8_t r = std::get<0>(color_tuple);
        uint8_t g = std::get<1>(color_tuple);
        uint8_t b = std::get<2>(color_tuple);

        if (is_16_bit) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            cppm_outfile.write(reinterpret_cast<const char*>(&r), 2);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            cppm_outfile.write(reinterpret_cast<const char*>(&g), 2);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            cppm_outfile.write(reinterpret_cast<const char*>(&b), 2);
        } else {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            cppm_outfile.write(reinterpret_cast<const char*>(&r), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            cppm_outfile.write(reinterpret_cast<const char*>(&g), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            cppm_outfile.write(reinterpret_cast<const char*>(&b), 1);
        }
    }

    int table_size = color_list.size();
    for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
        auto color_tuple = std::make_tuple(pixel_data.r[i], pixel_data.g[i], pixel_data.b[i]);
        int index = color_table[color_tuple];

        if (table_size <= 28) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            cppm_outfile.write(reinterpret_cast<const char*>(&index), 1);  // 1 byte <= 28 colors
        } else if (table_size <= 216) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            cppm_outfile.write(reinterpret_cast<const char*>(&index), 2);  // 2 bytes <= 216 colors
        } else if (table_size <= 65536) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            cppm_outfile.write(reinterpret_cast<const char*>(&index), 4);  // 4 bytes <= 232 colors
        } else {
            std::cerr << "Error: Color table too large." << '\n';
            exit(1);
        }
    }
}

void DimensionChange( ImageDimensions & original_dimension,  SoA & pixel_Data,
                     const ImageDimensions & new_dimension) {
  const int pixel_count = new_dimension.width * new_dimension.height;

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
      x_floor = floor(new_x);
      x_ceil = ceil(new_x);
      new_y =  static_cast<double>( i * original_dimension.height) / static_cast<double>(new_dimension.height);
      y_floor = floor(new_y);
      y_ceil = ceil(new_y);

      coordenadas = {new_x, x_floor, x_ceil, new_y , y_floor, y_ceil};
      //We write the new data in our new image
      std::vector<uint16_t> new_data = interpolacion_colores(pixel_Data, coordenadas, i , original_dimension);
      new_pixel_data.r[static_cast<unsigned long long int> (i  + (j * new_dimension.width))] = new_data[0];
      new_pixel_data.g[static_cast<unsigned long long int> (i  + (j * new_dimension.width))] = new_data[1];
      new_pixel_data.b[static_cast<unsigned long long int> (i  + (j * new_dimension.width))] = new_data[2];
    }
  }


}

std::vector<uint16_t> interpolacion_colores ( const SoA &pixel_Data, const std::vector<double> &coordenadas , const int width_counter , const ImageDimensions &original_dimension ) {
              std::vector<uint16_t> new_colors ;

              // --- r
              std::vector<double> first_point = {coordenadas[1], coordenadas[4], static_cast<double>(pixel_Data.r[static_cast<unsigned long long int>(
                            static_cast<long>(coordenadas[1] + coordenadas[4]) *
                            original_dimension.width)])};
              std::vector<double> second_point = {coordenadas[2], coordenadas[4], static_cast<double>(pixel_Data.r[static_cast<unsigned long long int>(
                                        static_cast<long>(coordenadas[2] + coordenadas[4]) *
                                        original_dimension.width)])};
              double color_c1 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[1], coordenadas[ 4 + 1 ],static_cast<double>(pixel_Data.r[static_cast<unsigned long long int>(
                                            static_cast<long>(coordenadas[1] + coordenadas[4 + 1]) *
                                            original_dimension.width)])};
              second_point = {coordenadas[2], coordenadas[4 + 1], static_cast<double>(pixel_Data.r[static_cast<unsigned long long int>(
                                             static_cast<long>(coordenadas[2] + coordenadas[4 + 1]) *
                                             original_dimension.width)])};
              double color_c2 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[0], coordenadas[4], color_c1};
              second_point = {coordenadas[0],coordenadas[4 + 1], color_c2};

              new_colors.push_back( static_cast<uint16_t>(interpolacion(first_point, second_point, width_counter)));
              // --g
              first_point = {coordenadas[1], coordenadas[4], static_cast<double>(pixel_Data.g[static_cast<unsigned long long int>(
                                            static_cast<long>(coordenadas[1] + coordenadas[4]) *
                                            original_dimension.width)])};
              second_point = {coordenadas[2], coordenadas[4], static_cast<double>(pixel_Data.g[static_cast<unsigned long long int>(
                                             static_cast<long>(coordenadas[2] + coordenadas[4]) *
                                             original_dimension.width)])};
              color_c1 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[1], coordenadas[4 +1], static_cast<double>(pixel_Data.g[static_cast<unsigned long long int>(
                                            static_cast<long>(coordenadas[1] + coordenadas[4 + 1]) *
                                            original_dimension.width)])};
              second_point = {coordenadas[2], coordenadas[4 + 1], static_cast<double>(pixel_Data.g[static_cast<unsigned long long int>(
                                             static_cast<long>(coordenadas[2] + coordenadas[4 + 1]) *
                                             original_dimension.width)])};
              color_c2 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[0], coordenadas[4], color_c1};
              second_point = {coordenadas[0], coordenadas[4 + 1], color_c2};
              new_colors.push_back(static_cast<uint16_t>(interpolacion(first_point, second_point, width_counter)));
              // --b
              first_point = {coordenadas[1], coordenadas[4], static_cast<double>(pixel_Data.b[static_cast<unsigned long long int>(
                                            static_cast<long>(coordenadas[1] + coordenadas[4]) *
                                            original_dimension.width)])};
              second_point = {coordenadas[2], coordenadas[4], static_cast<double>(pixel_Data.b[static_cast<unsigned long long int>(
                                             static_cast<long>(coordenadas[2] + coordenadas[4]) *
                                             original_dimension.width)])};
              color_c1 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[1], coordenadas[4 + 1], static_cast<double>(pixel_Data.b[static_cast<unsigned long long int>(
                                            static_cast<long>(coordenadas[1] + coordenadas[4 +1]) *
                                            original_dimension.width)])};
              second_point = {coordenadas[2], coordenadas[4 + 1 ], static_cast<double>(pixel_Data.b[static_cast<unsigned long long int>(
                                             static_cast<long>(coordenadas[2] + coordenadas[4 +1 ]) *
                                             original_dimension.width)])};
              color_c2 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[0], coordenadas[4], color_c1};
              second_point = {coordenadas[0], coordenadas[4 + 1], color_c2};
              new_colors.push_back( static_cast<uint16_t>(interpolacion(first_point, second_point, width_counter)));

  return new_colors;

}

double interpolacion( const std::vector<double>  &first_point ,const std::vector<double> &second_point , const  int y_value) {
      //Formula for getting the z ( color) value of the interpolation of two thredimensional points
      return ( first_point[2] + (( second_point[2] - first_point[2]) * ((y_value - first_point[1]) / ( second_point[1] - first_point[0]))));

    }

