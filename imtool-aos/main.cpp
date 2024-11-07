#include "common/progargs.hpp"
#include "functions.hpp"

#include <cstdlib>
#include <fstream>
#include <gsl/gsl>
#include <iostream>
#include <string>
#include <vector>

uint16_t MAX_COLOR_VALUE16 = 65535;

int main(int argc, const char *argv[]) {

  checkNumberArgs(argc); // Comprobar que el número de argumentos es correcto

  gsl::span const args{argv, gsl::narrow<std::size_t>(argc)}; // Creamos la vista

  switch (args[3]) {
    case "info":
      break;

    case "maxlevel":
      break;

    case "resize":
      break;

    case "cutfreq":
      break;

    case "compress":
      break;

    default:
      std::cerr << "Error: Invalid command: " << args[3] << "\n";
      exit(-1);
  }

  if (args[3] =="maxlevel") {
      if (argc !=5) {
          std::cerr << "Error: Invalid number of arguments for maxlevel: " << (argc - 4) << "\n";
      exit(-1);
      }
  }
  int new_maxlevel= 0;
  try {
      new_maxlevel = std::stoi(args[4]);
  } catch (const std::invalid_argument &){
      std::cerr << "Error: Invalid argument for maxlevel: " << args[4] << "\n";
      exit(-1);
  }
  if (new_maxlevel < 0 || new_maxlevel > MAX_COLOR_VALUE16){
      std::cerr << "Error: Invalid maxlevel value: " << new_maxlevel << "\n";
      exit(-1);
  }

 std::ifstream infile(args[1], std::ios::binary);  // abrir en modo binario
  if (!infile) {
      std::cerr << "Error: Could not open file " << args[1] << "\n";
      return 1;
  }

  std::ofstream outfile(args[2], std::ios::binary);
  if (!outfile) {
      std::cerr << "Error: Could not open file " << args[2] << "\n";
      infile.close();
      return 1;
  }

  // Variables del header
  std::string magic_number;
  int width = 0;
  int height = 0;
  int max_color = 0;

  // INFO COMMAND
  get_header(infile, magic_number, width, height, max_color);

  // Tamaño en pixeles de la imagen
  int pixel_count = width * height;

  // Array of Structures
  std::vector<Pixel> pixel_data(static_cast<std::size_t>(pixel_count));

  // Determinar la longitud de cada pixel (2 bytes si max_color > 256; else: 1)
  bool is_16_bit = max_color > 255;

  // RELLENAR EL ARRAY OF STRUCTURES CON LOS PIXELES
  get_pixels(infile, pixel_data, pixel_count, is_16_bit);

  if (args[3] == "maxlevel") {
      maxlevel(pixel_data, new_maxlevel, max_color, is_16_bit);
  }
  // Escribir en outfile, INFO COMMAND
  write_info(outfile, magic_number, width, height, max_color, pixel_data, is_16_bit);

  std::ofstream cppm_outfile(args[3], std::ios::binary);

  // COMPRESS COMMAND
  write_cppm(cppm_outfile, pixel_data, width, height, max_color);

  infile.close();
  outfile.close();
  cppm_outfile.close();
  return 0;
};
