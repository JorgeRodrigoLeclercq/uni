#include "functions.hpp"
#include <algorithm> // For std::clamp
#include <bits/algorithmfwd.h>

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

// Guardar los pixeles de la imagen ppm en una estructura SoA
void get_pixels(std::ifstream &infile, SoA &pixel_data, int pixel_count, bool is_16_bit) {
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
            pixel_data.r[i] = r1 | (r2 << 8);
            pixel_data.g[i] = g1 | (g2 << 8);
            pixel_data.b[i] = b1 | (b2 << 8);
        }
    } else {
        // max_color > 255 y por ende, bits en pixel == 1
        for (int i = 0; i < pixel_count; ++i) {
            infile.read(reinterpret_cast<char*>(&pixel_data.r[i]), 1);
            infile.read(reinterpret_cast<char*>(&pixel_data.g[i]), 1);
            infile.read(reinterpret_cast<char*>(&pixel_data.b[i]), 1);
        }
    }

    // Log pixels
    std::cout << "First few pixels (RGB values):" << std::endl;
    for (int i = 0; i < std::min(10, pixel_count); ++i) {
        std::cout << "Pixel " << i << ": "
                  << "R = " << static_cast<int>(pixel_data.r[i]) << ", "
                  << "G = " << static_cast<int>(pixel_data.g[i]) << ", "
                  << "B = " << static_cast<int>(pixel_data.b[i]) << std::endl;
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
            outfile.write(reinterpret_cast<const char*>(&r1), 1);
            outfile.write(reinterpret_cast<const char*>(&r2), 1);
            outfile.write(reinterpret_cast<const char*>(&g1), 1);
            outfile.write(reinterpret_cast<const char*>(&g2), 1);
            outfile.write(reinterpret_cast<const char*>(&b1), 1);
            outfile.write(reinterpret_cast<const char*>(&b2), 1);
        }
    } else {
        for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
            outfile.write(reinterpret_cast<const char*>(&pixel_data.r[i]), 1);
            outfile.write(reinterpret_cast<const char*>(&pixel_data.g[i]), 1);
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
            cppm_outfile.write(reinterpret_cast<const char*>(&r), 2);
            cppm_outfile.write(reinterpret_cast<const char*>(&g), 2);
            cppm_outfile.write(reinterpret_cast<const char*>(&b), 2);
        } else {
            cppm_outfile.write(reinterpret_cast<const char*>(&r), 1);
            cppm_outfile.write(reinterpret_cast<const char*>(&g), 1);
            cppm_outfile.write(reinterpret_cast<const char*>(&b), 1);
        }
    }

    int table_size = color_list.size();
    for (std::size_t i = 0; i < pixel_data.r.size(); ++i) {
        auto color_tuple = std::make_tuple(pixel_data.r[i], pixel_data.g[i], pixel_data.b[i]);
        int index = color_table[color_tuple];

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


