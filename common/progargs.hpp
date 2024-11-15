// progargs.hpp
#ifndef PROGARGS_HPP
#define PROGARGS_HPP

#include "header.hpp"

#include <cstdint>
#include <fstream>
#include <gsl/span>
#include <iostream>

constexpr uint8_t EXTRA_ARGS = 5;

inline int checkNumberArgs(int const argc) {
  if (argc < 4) {
    std::cerr << "Error: Invalid number of arguments: " << argc - 1 << "\n";
    exit(-1); // Código de error si hay menos de 3 argumentos
  }
  return 0;
}

inline int checkInfoAndCompress(int const argc) {
  if (argc > 4) {
    std::cerr << "Error: Invalid number of arguments: " << argc - 1 << "\n";
    exit(-1); // Código de error si hay menos de 3 argumentos
  }
  return 0;
}

constexpr int MAX_COLOR_VALUE16 = 65535;
inline int checkMaxLevel(const std::string& arg) {
  int new_maxlevel = 0;
  try {
    new_maxlevel = std::stoi(arg);
  } catch (const std::invalid_argument&) {
    std::cerr << "Error: Invalid argument for maxlevel: " << arg << "\n";
    exit(-1);
  }
  if (new_maxlevel < 0 || new_maxlevel > MAX_COLOR_VALUE16) {
    std::cerr << "Error: Invalid maxlevel value: " << new_maxlevel << "\n";
    exit(-1);
  }
  return new_maxlevel;
}

inline void checkDimensions(ImageDimensions const& dimensions) {
  if (dimensions.width < 0 ){
    std::cerr << "Error: Invalid resize width: " << dimensions.width << "\n";
    exit(-1);
  }
  if (dimensions.height < 0) {
    std::cerr << "Error: Invalid resize height: " << dimensions.height << "\n";
    exit(-1);
  }
}

inline int checkCutFreq(gsl::span<char const *> args, int argc) {
  if (argc != EXTRA_ARGS) {
    std::cerr << "Error: Invalid number of arguments for cutfreq: " << argc - 1 << "\n";
    exit(1);
  }
  int n_colors = 0;
  try {
    n_colors = std::stoi(args[4]);
  } catch (const std::invalid_argument&) {
    std::cerr << "Error: Invalid cutfreq: " << args[4] << "\n";
    exit(1);
  }
  // Verificar si el número es positivo
  if (n_colors < 0) {
    std::cerr << "Error: cutfreq must be a positive number, but got: " << n_colors << "\n";
    exit(1); // Salir con un código de error
  }
  return n_colors;
}

#endif // PROGARGS_HPP
