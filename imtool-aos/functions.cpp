#include "functions.hpp"
#include <cmath>
#include <bits/algorithmfwd.h>


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

// Guardar los pixeles de la imagen ppm en una estructura AoS
void get_pixels(std::ifstream &infile, std::vector<Pixel> &pixel_data, unsigned long long pixel_count, bool is_16_bit) {
  const int BIT_SHIFT = 8;
  const int FIRST_PIXELS = 10;
    if (is_16_bit) {
        // max_color > 255 t por ende, bits en pixel == 2, en little-endian
        for (unsigned long long int i = 0; i < pixel_count; ++i) {
            uint8_t red1 = 0;
            uint8_t red2 = 0;
            uint8_t green1 = 0;
            uint8_t green2 = 0;
            uint8_t blue1 = 0;
            uint8_t blue2 = 0;
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&red1), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&red2), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&green1), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&green2), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&blue1), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&blue2), 1);

            // Little-endian
            pixel_data[i].channels.red = static_cast<uint16_t>(red1 | (red2 << BIT_SHIFT));
            pixel_data[i].channels.green = static_cast<uint16_t>(green1 | (green2 << BIT_SHIFT));
            pixel_data[i].channels.blue = static_cast<uint16_t>(blue1 | (blue2 << BIT_SHIFT));
        }
    } else {
        // max_color < 255 y por ende, bits en pixel == 1
        for (unsigned long long i = 0; i < pixel_count; ++i) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&pixel_data[i].channels.red), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&pixel_data[i].channels.green), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&pixel_data[i].channels.blue), 1);
        }
    }

    // Log pixels
    std::cout << "First few pixels (RGB values):" << '\n';
    for (unsigned long long int i = 0; i < std::min(static_cast<unsigned long long int>(FIRST_PIXELS), pixel_count); ++i) {
        std::cout << "Pixel " << i << ": "
                  << "R = " << static_cast<int>(pixel_data[static_cast<unsigned long long int>(i)].channels.red) << ", "
                  << "G = " << static_cast<int>(pixel_data[static_cast<unsigned long long int>(i)].channels.green) << ", "
                  << "B = " << static_cast<int>(pixel_data[static_cast<unsigned long long int>(i)].channels.blue) << '\n';
    }
}

// Ecribir la información de la imagen en el archivo de salida
void write_info(std::ofstream &outfile, const ImageHeader &header, const std::vector<Pixel> &pixel_data, bool is_16_bit) {
    const uint16_t MASK_BYTE = 0xFF;
    const int BITS_PER_BYTE = 8;
    outfile << header.magic_number << "\n";
    outfile << header.dimensions.width << " " << header.dimensions.height << "\n";
    outfile << header.max_color << "\n";

    if (is_16_bit) {
        for (const auto &pixel : pixel_data) {
            uint8_t red1 = pixel.channels.red & MASK_BYTE;
            uint8_t red2 = (pixel.channels.red >> BITS_PER_BYTE) & MASK_BYTE;
            uint8_t green1 = pixel.channels.green & MASK_BYTE;
            uint8_t green2 = (pixel.channels.green >> BITS_PER_BYTE) & MASK_BYTE;
            uint8_t blue1 = pixel.channels.blue & MASK_BYTE;
            uint8_t blue2 = (pixel.channels.blue >> BITS_PER_BYTE) & MASK_BYTE;
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
        for (const auto &pixel : pixel_data) {
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            outfile.write(reinterpret_cast<const char*>(&pixel.channels.red), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            outfile.write(reinterpret_cast<const char*>(&pixel.channels.green), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            outfile.write(reinterpret_cast<const char*>(&pixel.channels.blue), 1);
        }
    }
}

void write_cppm(std::ofstream &cppm_outfile, const ImageHeader &header, const std::vector<Pixel> &pixel_data) {
  const uint8_t MAX_COLOR_VALUE8 = 255;
  const int MAX_COLORS_1BYTE = 28;
  const int MAX_COLORS_2BYTES = 216;
  const int MAX_COLORS_4BYTES = 65536;
    // Mapear cada índice a su color
    std::map<Pixel, int> color_table;
    std::vector<Pixel> color_list;

    int color_index = 0;
    for (const auto &pixel : pixel_data) {
        if (color_table.find(pixel) == color_table.end()) {
            color_table[pixel] = color_index++;
            color_list.push_back(pixel);
        }
    }

    // CPPM header
    cppm_outfile << "C6 " << header.dimensions.width << " " << header.dimensions.height << " " << header.max_color << " " << color_list.size() << "\n";

    bool is_16_bit = header.max_color > MAX_COLOR_VALUE8;
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
    int table_size = color_list.size();
    for (const auto &pixel : pixel_data) {
      int index = color_table[pixel];
      if (table_size <= MAX_COLORS_1BYTE) {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        cppm_outfile.write(reinterpret_cast<const char*>(&index), 1);  // 1 byte <= 28 colors
      } else if (table_size <= MAX_COLORS_2BYTES) {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        cppm_outfile.write(reinterpret_cast<const char*>(&index), 2);  // 2 bytes <= 216 colors
      } else if (table_size <= MAX_COLORS_4BYTES) {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        cppm_outfile.write(reinterpret_cast<const char*>(&index), 4);  // 4 bytes <= 232 colors
      } else {
        std::cerr << "Error: Color table too large." << '\n';
        exit(1);
      }
    }
}
template<typename T>
T clamp(const T& value, const T& low, const T& high) {
    return (value < low) ? low : (value > high) ? high : value;
}

constexpr int MAX_COLOR_8BIT = 255;

void maxlevel(int new_maxlevel, bool& is_16_bit, gsl::span<Pixel> &pixel_data, ImageHeader &header) {
  std::cout << "Previous max_color: " << header.max_color << ", New maxlevel: " << new_maxlevel << '\n';

  // Determinar si la salida será de 8 o 16 bits
  is_16_bit = new_maxlevel > MAX_COLOR_8BIT;

  for (auto& pixel : pixel_data) {
    // Escalar cada componente sin redondeo
    uint16_t r_scaled = static_cast<uint16_t>(
        clamp(static_cast<int>(static_cast<float>(pixel.channels.red) * static_cast<float>(new_maxlevel) / static_cast<float>(header.max_color)),
              0, new_maxlevel));
    uint16_t g_scaled = static_cast<uint16_t>(
        clamp(static_cast<int>(static_cast<float>(pixel.channels.green) * static_cast<float>(new_maxlevel) / static_cast<float>(header.max_color)),
              0, new_maxlevel));
    uint16_t b_scaled = static_cast<uint16_t>(
        clamp(static_cast<int>(static_cast<float>(pixel.channels.blue) * static_cast<float>(new_maxlevel) / static_cast<float>(header.max_color)),
              0, new_maxlevel));

    // Asignar valores escalados
    pixel.channels.red = is_16_bit ? r_scaled : static_cast<uint8_t>(r_scaled);
    pixel.channels.green = is_16_bit ? g_scaled : static_cast<uint8_t>(g_scaled);
    pixel.channels.blue = is_16_bit ? b_scaled : static_cast<uint8_t>(b_scaled);
  }

  // Actualizar max_color al nuevo nivel máximo
  header.max_color = new_maxlevel;
  std::cout << "Updated max_color: " << header.max_color << '\n'; // Depuración
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
              x_floor = floor(new_x);
              x_ceil = ceil(new_x);
              new_y =  static_cast<double>( i * original_dimension.height) / static_cast<double>(new_dimension.height);
              y_floor = floor(new_y);
              y_ceil = ceil(new_y);

              coordenadas = {new_x, x_floor, x_ceil, new_y , y_floor, y_ceil};
              //We write the new data in our new image
              new_pixel_data[static_cast<unsigned long long int>(i + (j * new_dimension.width))] = interpolacion_colores(pixel_Data, coordenadas, i , original_dimension);
            }
      }
}
