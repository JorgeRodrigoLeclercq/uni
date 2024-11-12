#ifndef COMPRESS_HPP
#define COMPRESS_HPP
#include "info.hpp"

void write_cppm(std::ofstream& cppm_outfile, ImageHeader& header, SoA& pixel_data);

#endif // COMPRESS_HPP