// progargs.hpp
#ifndef PROGARGS_HPP
#define PROGARGS_HPP

#include <iostream>
#include <fstream>

inline int checkNumberArgs(int const argc) {
  if (argc < 4) {
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

inline int checkWidthArgs(int const width) {
  if (width < 0 ){
    std::cerr << "Error: Invalid resize width: " << width << "\n";
    exit(-1);
  }
  return 0;
}

inline int checkHeightArgs(int const heigth) {
  if (heigth< 0 ){
    std::cerr << "Error: Invalid resize height: " << heigth << "\n";
    exit(-1);
  }
  return 0;
}



#endif // PROGARGS_HPP
