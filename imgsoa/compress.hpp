#ifndef COMPRESS_HPP
#define COMPRESS_HPP

#include "info.hpp"

#include <map>

void write_color_table(std::ofstream &outfile, SoA& pixel_data,
                       std::map<std::tuple<uint8_t, uint8_t, uint8_t>, int> const &color_table);

void compress(std::ofstream& cppm_outfile, ImageHeader & header, SoA& pixel_data);

#endif // COMPRESS_HPP