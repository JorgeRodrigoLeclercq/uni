#ifndef HEADER_HPP
#define HEADER_HPP

#include <fstream>
#include <iostream>
#include <string>

constexpr int DEFAULT_MAX_COLOR = 255;

struct ImageDimensions {
  int width;
  int height;
};

struct ImageHeader {
  std::string magic_number;
  ImageDimensions dimensions;
  int max_color;

  explicit ImageHeader(std::string magic = "", ImageDimensions const dims = {.width=0, .height=0},
                       int const max_c = DEFAULT_MAX_COLOR)
    : magic_number(std::move(magic)), dimensions(dims), max_color(max_c) {}
};

// Guardar la información del header de la imagen ppm en magic_number, width, height y max_color
inline void get_header(std::ifstream &infile, ImageHeader &header) {
  constexpr int MAX_IGNORE_CHARS = 256;
  // Leer el header
  infile >> header.magic_number >> header.dimensions.width >> header.dimensions.height >>
      header.max_color;

  if (header.magic_number != "P6") {
    std::cerr << "Error: This program only supports P6 (binary) format" << '\n';
    exit(1);
  }

  // Saltar espacios blancos
  infile.ignore(MAX_IGNORE_CHARS, '\n');
}

#endif // HEADER_HPP