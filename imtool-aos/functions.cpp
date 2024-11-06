#include "functions.hpp"
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

// Guardar los pixeles de la imagen ppm en una estructura AoS
void get_pixels(std::ifstream &infile, std::vector<Pixel> &pixel_data, int pixel_count, bool is_16_bit) {
    if (is_16_bit) {
        // max_color > 255 t por ende, bits en pixel == 2, en little-endian
        for (int i = 0; i < pixel_count; ++i) {
            uint8_t r1, r2, g1, g2, b1, b2;
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
            pixel_data[i].r = r1 | (r2 << 8);
            pixel_data[i].g = g1 | (g2 << 8);
            pixel_data[i].b = b1 | (b2 << 8);
        }
    } else {
        // max_color < 255 y por ende, bits en pixel == 1
        for (int i = 0; i < pixel_count; ++i) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&pixel_data[i].r), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&pixel_data[i].g), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            infile.read(reinterpret_cast<char*>(&pixel_data[i].b), 1);
        }
    }

    // Log pixels
    std::cout << "First few pixels (RGB values):" << std::endl;
    for (int i = 0; i < std::min(10, pixel_count); ++i) {
        std::cout << "Pixel " << i << ": "
                  << "R = " << static_cast<int>(pixel_data[i].r) << ", "
                  << "G = " << static_cast<int>(pixel_data[i].g) << ", "
                  << "B = " << static_cast<int>(pixel_data[i].b) << std::endl;
    }
}

// Ecribir la información de la imagen en el archivo de salida
void write_info(std::ofstream &outfile, const std::string &magic_number, int width, int height, int max_color, const std::vector<Pixel> &pixel_data, bool is_16_bit) {
    outfile << magic_number << "\n";
    outfile << width << " " << height << "\n";
    outfile << max_color << "\n";

    if (is_16_bit) {
        for (const auto &pixel : pixel_data) {
            uint8_t r1 = pixel.r & 0xFF;
            uint8_t r2 = (pixel.r >> 8) & 0xFF;
            uint8_t g1 = pixel.g & 0xFF;
            uint8_t g2 = (pixel.g >> 8) & 0xFF;
            uint8_t b1 = pixel.b & 0xFF;
            uint8_t b2 = (pixel.b >> 8) & 0xFF;
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
        for (const auto &pixel : pixel_data) {
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            outfile.write(reinterpret_cast<const char*>(&pixel.r), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            outfile.write(reinterpret_cast<const char*>(&pixel.g), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            outfile.write(reinterpret_cast<const char*>(&pixel.b), 1);
        }
    }
}

void write_cppm(std::ofstream &cppm_outfile, const std::vector<Pixel> &pixel_data, int width, int height, int max_color) {
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
    cppm_outfile << "C6 " << width << " " << height << " " << max_color << " " << color_list.size() << "\n";

    bool is_16_bit = max_color > 255;
    for (const auto &color : color_list) {
        if (is_16_bit) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            cppm_outfile.write(reinterpret_cast<const char*>(&color.r), 2);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            cppm_outfile.write(reinterpret_cast<const char*>(&color.g), 2);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            cppm_outfile.write(reinterpret_cast<const char*>(&color.b), 2);
        } else {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            cppm_outfile.write(reinterpret_cast<const char*>(&color.r), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            cppm_outfile.write(reinterpret_cast<const char*>(&color.g), 1);
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            cppm_outfile.write(reinterpret_cast<const char*>(&color.b), 1);
        }
    }

    int table_size = color_list.size();
    for (const auto &pixel : pixel_data) {
        int index = color_table[pixel];
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
template<typename T>
T clamp(const T& value, const T& low, const T& high) {
    return (value < low) ? low : (value > high) ? high : value;
}
constexpr int MAX_COLOR_8BIT = 255;
void maxlevel(gsl::span<Pixel> pixel_data, int new_maxlevel, int& max_color, bool& is_16_bit) {
  std::cout << "Previous max_color: " << max_color << ", New maxlevel: " << new_maxlevel << '\n';

  // Determinar si la salida será de 8 o 16 bits
  is_16_bit = new_maxlevel > MAX_COLOR_8BIT;

  for (auto& pixel : pixel_data) {
    // Escalar cada componente sin redondeo
    uint16_t r_scaled = static_cast<uint16_t>(
        clamp(static_cast<int>(static_cast<float>(pixel.r) * static_cast<float>(new_maxlevel) / static_cast<float>(max_color)),
              0, new_maxlevel));
    uint16_t g_scaled = static_cast<uint16_t>(
        clamp(static_cast<int>(static_cast<float>(pixel.g) * static_cast<float>(new_maxlevel) / static_cast<float>(max_color)),
              0, new_maxlevel));
    uint16_t b_scaled = static_cast<uint16_t>(
        clamp(static_cast<int>(static_cast<float>(pixel.b) * static_cast<float>(new_maxlevel) / static_cast<float>(max_color)),
              0, new_maxlevel));

    // Asignar valores escalados
    pixel.r = is_16_bit ? r_scaled : static_cast<uint8_t>(r_scaled);
    pixel.g = is_16_bit ? g_scaled : static_cast<uint8_t>(g_scaled);
    pixel.b = is_16_bit ? b_scaled : static_cast<uint8_t>(b_scaled);
  }

  // Actualizar max_color al nuevo nivel máximo
  max_color = new_maxlevel;
}









 /* double interpolacion(std::vector<double>  &first_point , std::vector<double> & second_point , int y_value) {
   //Formula for getting the z ( color) value of the interpolation of two thredimensional points
      return ( first_point[2] + (( second_point[2] - first_point[2]) * ((y_value - first_point[1]) / ( second_point[1] - first_point[0]))));

}

Pixel interpolacion_colores ( std::vector<Pixel> &pixel_Data, std::vector<double> &coordenadas , int width_counter , Dimension &original_dimension ) {

  Pixel pixel;
              std::vector<double> first_point = {coordenadas[1], coordenadas[4], (double)pixel_Data[static_cast<long>( coordenadas[1] + coordenadas[4])* original_dimension.width].r};
              std::vector<double> second_point = {coordenadas[2], coordenadas[4], (double)pixel_Data[static_cast<long>( coordenadas[2] + coordenadas[4]) * original_dimension.width].r};
              double color_c1 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[1], coordenadas[5],pixel_Data[static_cast<long> (coordenadas[1] + coordenadas[5])* original_dimension.width].r};
              second_point = {coordenadas[2], coordenadas[5], pixel_Data[static_cast<long>( coordenadas[2]+ coordenadas[5])* original_dimension.width].r};
              double color_c2 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[0], coordenadas[4], color_c1};
              second_point = {coordenadas[0],coordenadas[5], color_c2};
              pixel.r      = static_cast<uint16_t>(interpolacion(first_point, second_point, width_counter));
              // --g
              first_point = {coordenadas[1], coordenadas[4], pixel_Data[static_cast<long>( coordenadas[1] + coordenadas[4])* original_dimension.width].g};
              second_point = {coordenadas[2], coordenadas[4], pixel_Data[static_cast<long>( coordenadas[2] + coordenadas[4]) * original_dimension.width].g};
              color_c1 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[1], coordenadas[5], pixel_Data[static_cast<long>( coordenadas[1] + coordenadas[5])*original_dimension.width].g};
              second_point = {coordenadas[2], coordenadas[5], pixel_Data[static_cast<long>( coordenadas[2] + coordenadas[5])* original_dimension.width].g};
              color_c2 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[0], coordenadas[4], color_c1};
              second_point = {coordenadas[0], coordenadas[5], color_c2};
              pixel.g = static_cast<uint16_t>(interpolacion(first_point, second_point, width_counter));
              // --b
              first_point = {coordenadas[1], coordenadas[4], pixel_Data[static_cast<long>( coordenadas[1]+ coordenadas[4])* original_dimension.width].b};
              second_point = {coordenadas[2], coordenadas[4], pixel_Data[static_cast<long>( coordenadas[2] + coordenadas[4]) * original_dimension.width].b};
              color_c1 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[1], coordenadas[5], pixel_Data[static_cast<long>( coordenadas[1] + coordenadas[5])* original_dimension.width].b};
              second_point = {coordenadas[2], coordenadas[5], pixel_Data[static_cast<long>( coordenadas[2] + coordenadas[5])* original_dimension.width].b};
              color_c2 = interpolacion(first_point, second_point, width_counter);
              first_point = {coordenadas[0], coordenadas[4], color_c1};
              second_point = {coordenadas[0], coordenadas[5], color_c2};
              pixel.b = static_cast<uint16_t>(interpolacion(first_point, second_point, width_counter));

  return pixel;

}

  void DimensionChange(Dimension& original_dimension, std::vector<Pixel> &pixel_Data , Dimension& new_dimension) {
      std::vector<Pixel> new_pixel_data(static_cast<std::size_t>(new_dimension.width * new_dimension.height));
      Pixel pixel; pixel.r = 0; pixel.g = 0; pixel.b = 0;
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
              new_pixel_data[i + (j * i)] = interpolacion_colores(pixel_Data, coordenadas, i , original_dimension);
            }



      }

} */
