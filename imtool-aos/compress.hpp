#ifndef COMPRESS_HPP
#define COMPRESS_HPP
#include "functions.hpp"

void write_color_table(std::ofstream &outfile, const std::vector<Pixel> &pixel_data, const std::map<Pixel, int> &color_table);

void write_cppm(std::ofstream &cppm_outfile, const ImageHeader &header, const std::vector<Pixel> &pixel_data);

#endif // COMPRESS_HPP
