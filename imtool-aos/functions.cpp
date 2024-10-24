#include "functions.hpp"
#include <cmath>


// Guardar la información del header de la imagen ppm en magic_number, width, height y max_color
void get_header(std::ifstream &infile, std::string &magic_number, int &width, int &height, int &max_color) {
    // Leer el header
    infile >> magic_number >> width >> height >> max_color;

    if (magic_number != "P6") {
        std::cerr << "Error: This program only supports P6 (binary) format" << std::endl;
        exit(1);
    }

    // Saltar espacios blancos
    infile.ignore(256, '\n');

    // Log el header
    std::cout << "Magic Number: " << magic_number << std::endl;
    std::cout << "Width: " << width << std::endl;
    std::cout << "Height: " << height << std::endl;
    std::cout << "Max Color: " << max_color << std::endl;
}

// Guardar los pixeles de la imagen ppm en una estructura AoS
void get_pixels(std::ifstream &infile, std::vector<Pixel> &pixel_data, int pixel_count, bool is_16_bit) {
    if (is_16_bit) {
        // max_color > 255 t por ende, bits en pixel == 2, en little-endian
        for (int i = 0; i < pixel_count; ++i) {
            uint8_t r1, r2, g1, g2, b1, b2;
            infile.read(reinterpret_cast<char*>(&r1), 1);
            infile.read(reinterpret_cast<char*>(&r2), 1);
            infile.read(reinterpret_cast<char*>(&g1), 1);
            infile.read(reinterpret_cast<char*>(&g2), 1);
            infile.read(reinterpret_cast<char*>(&b1), 1);
            infile.read(reinterpret_cast<char*>(&b2), 1);

            // Little-endian
            pixel_data[i].r = r1 | (r2 << 8);
            pixel_data[i].g = g1 | (g2 << 8);
            pixel_data[i].b = b1 | (b2 << 8);
        }
    } else {
        // max_color < 255 y por ende, bits en pixel == 1
        for (int i = 0; i < pixel_count; ++i) {
            infile.read(reinterpret_cast<char*>(&pixel_data[i].r), 1);
            infile.read(reinterpret_cast<char*>(&pixel_data[i].g), 1);
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
            outfile.write(reinterpret_cast<const char*>(&r1), 1);
            outfile.write(reinterpret_cast<const char*>(&r2), 1);
            outfile.write(reinterpret_cast<const char*>(&g1), 1);
            outfile.write(reinterpret_cast<const char*>(&g2), 1);
            outfile.write(reinterpret_cast<const char*>(&b1), 1);
            outfile.write(reinterpret_cast<const char*>(&b2), 1);
        }
    } else {
        for (const auto &pixel : pixel_data) {
            outfile.write(reinterpret_cast<const char*>(&pixel.r), 1);
            outfile.write(reinterpret_cast<const char*>(&pixel.g), 1);
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
            cppm_outfile.write(reinterpret_cast<const char*>(&color.r), 2);
            cppm_outfile.write(reinterpret_cast<const char*>(&color.g), 2);
            cppm_outfile.write(reinterpret_cast<const char*>(&color.b), 2);
        } else {
            cppm_outfile.write(reinterpret_cast<const char*>(&color.r), 1);
            cppm_outfile.write(reinterpret_cast<const char*>(&color.g), 1);
            cppm_outfile.write(reinterpret_cast<const char*>(&color.b), 1);
        }
    }

    int table_size = color_list.size();
    for (const auto &pixel : pixel_data) {
        int index = color_table[pixel];
        if (table_size <= 28) {
            cppm_outfile.write(reinterpret_cast<const char*>(&index), 1);  // 1 byte <= 28 colors
        } else if (table_size <= 216) {
            cppm_outfile.write(reinterpret_cast<const char*>(&index), 2);  // 2 bytes <= 216 colors
        } else if (table_size <= 65536) {
            cppm_outfile.write(reinterpret_cast<const char*>(&index), 4);  // 4 bytes <= 232 colors
        } else {
            std::cerr << "Error: Color table too large." << std::endl;
            exit(1);
        }
    }
}
void maxlevel(std::vector<Pixel> &pixel_data, int new_maxlevel, int max_color) {
    if (new_maxlevel <=0) {
        std::cerr << "Error: new_maxlevel debe ser mayor que 0" << std::endl;
        exit(1);
    }
    for (auto& pixel : pixel_data) {
        // Escalar cada componente y asegurarse de que esté dentro del rango
        pixel.r = clamp(static_cast<int>(std::floor(pixel.r * new_maxlevel / static_cast<float>(max_color))), 0, new_maxlevel);
        pixel.g = clamp(static_cast<int>(std::floor(pixel.g * new_maxlevel / static_cast<float>(max_color))), 0, new_maxlevel);
        pixel.b = clamp(static_cast<int>(std::floor(pixel.b * new_maxlevel / static_cast<float>(max_color))), 0, new_maxlevel);
    }
}