#ifndef COMPRESS_HPP
#define COMPRESS_HPP

#include "info.hpp"

#include <map>

void write_color_table(std::ofstream &outfile, const std::vector<Pixel> &pixel_data, const std::map<Pixel, int> &color_table);

void compress(std::ofstream &cppm_outfile, ImageHeader &header, const std::vector<Pixel> &pixel_data);

#endif // COMPRESS_HPP
