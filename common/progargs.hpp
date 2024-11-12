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
