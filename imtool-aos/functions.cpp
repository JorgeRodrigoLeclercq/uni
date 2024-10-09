#include "functions.hpp"

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