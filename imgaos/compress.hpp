#ifndef COMPRESS_HPP
#define COMPRESS_HPP

#include "info.hpp"

#include <map>

void write_cppm_header(std::ofstream &outfile, const ImageHeader &header, size_t color_table_size);

void write_color_table(std::ofstream &outfile, const std::vector<Pixel> &unique_colors, int max_color);

void write_compressed_pixel_data(std::ofstream &outfile, const std::vector<Pixel> &pixel_data, const std::map<Pixel, uint32_t> &color_table, size_t index_size);

void compress(std::ofstream &outfile, const ImageHeader &header, const std::vector<Pixel> &pixel_data);

#endif // COMPRESS_HPP
