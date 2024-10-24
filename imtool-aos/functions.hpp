#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

// Array of Structures (AoS)
struct Pixel{
    uint8_t r, g, b;
};

// Guardar la información del header de la imagen ppm en magic_number, width, height y max_color
void get_header(std::ifstream &infile, std::string &magic_number, int &width, int &height, int &max_color);

// Guardar los pixeles de la imagen ppm en una estructura AoS
void get_pixels(std::ifstream &infile, std::vector<Pixel> &pixel_data, int pixel_count, bool is_16_bit);

// Ecribir la información de la imagen en el archivo de salida
void write_info(std::ofstream &outfile, const std::string &magic_number, int width, int height, int max_color, const std::vector<Pixel> &pixel_data, bool is_16_bit);
#endif //FUNCTIONS_HPP