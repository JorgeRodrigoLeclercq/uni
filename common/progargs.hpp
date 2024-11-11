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



#endif // PROGARGS_HPP
