#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <map>
#include <variant>
#include <cstdint>

struct Pixel {
    uint16_t r;  
    uint16_t g; 
    uint16_t b;

    Pixel(uint16_t red = 0, uint16_t green = 0, uint16_t blue = 0)
        : r(red), g(green), b(blue) {}
    
    bool operator<(const Pixel &other) const {
        return std::tie(r, g, b) < std::tie(other.r, other.g, other.b);
    }
};

struct Dimension {
    int height;
    int width;

    Dimension(int height = 0, int width = 0)
        : height(height), width(width) {}

};

// Guardar la información del header de la imagen ppm en magic_number, width, height y max_color
void get_header(std::ifstream &infile, std::string &magic_number, int &width, int &height, int &max_color);

// Guardar los pixeles de la imagen ppm en una estructura AoS
void get_pixels(std::ifstream &infile, std::vector<Pixel> &pixel_data, int pixel_count, bool is_16_bit);

// Ecribir la información de la imagen en el archivo de salida
void write_info(std::ofstream &outfile, const std::string &magic_number, int width, int height, int max_color, const std::vector<Pixel> &pixel_data, bool is_16_bit);

// Comprimir la imagen
void write_cppm(std::ofstream &cppm_outfile, const std::vector<Pixel> &pixel_data, int width, int height, int max_color);

void maxlevel(std::vector<Pixel> &pixel_data, int new_maxlevel, int max_color);

double interpolacion(std::vector<double>  &first_point , std::vector<double> & second_point , int y_value);

Pixel interpolacion_colores ( std::vector<Pixel> &pixel_Data, std::vector<double> &coordenadas , int width_counter , Dimension &original_dimension );

void DimensionChange(Dimension& original_dimension, std::vector<Pixel> &pixel_Data , Dimension& new_dimension);
#endif //FUNCTIONS_HPP

