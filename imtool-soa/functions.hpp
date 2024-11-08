#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <map>
#include <algorithm>
#include <tuple>

struct ImageDimensions {
    int width;
    int height;
};

// Structure of Arrays (SoA)
struct SoA {
    std::vector<uint8_t> r, g, b;  // arrays distintos para el rojo, el verde y el azul
};
template<typename T>
T clamp(const T& value, const T& low, const T& high) {
    return (value < low) ? low : (value > high) ? high : value;
}
void get_header(std::ifstream &infile, std::string &magic_number, int &width, int &height, int &max_color);
void get_pixels(std::ifstream &infile, SoA &pixel_data, int pixel_count, bool is_16_bit);
void write_info(std::ofstream &outfile, const std::string &magic_number, int width, int height, int max_color, SoA &pixel_data, bool is_16_bit);
void write_cppm(std::ofstream &cppm_outfile, SoA &pixel_data, int width, int height, int max_color);
double interpolacion(const std::vector<double> &first_point, const std::vector<double> &second_point, int y_value);

std::vector<uint16_t> interpolacion_colores(SoA &pixel_Data, std::vector<double> &coordenadas , int width_counter , ImageDimensions &original_dimension );

void DimensionChange(ImageDimensions& original_dimension, std::vector<SoA> &pixel_data, ImageDimensions& new_dimension);
#endif // FUNCTIONS_HPP
