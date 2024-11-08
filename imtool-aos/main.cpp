#include "../common/progargs.hpp"
#include "functions.hpp"

#include <cstdlib>
#include <fstream>
#include <gsl/gsl>
#include <iostream>
#include <string>
#include <vector>

constexpr uint8_t MAX_COLOR_VALUE8 = 255;
constexpr uint16_t MAX_COLOR_VALUE16 = 65535;

int main(int argc, const char *argv[]) {

  checkNumberArgs(argc); // Comprobar que el número de argumentos es correcto para todas las funciones

  gsl::span const args{argv, gsl::narrow<std::size_t>(argc)}; // Creamos la vista

  /*switch (args[3]) {
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
*/










  if (std::string(args[3]) == "maxlevel") {
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
  ImageHeader header;

  // INFO COMMAND
  get_header(infile, header);

  // Tamaño en pixeles de la imagen
  auto pixel_count =
      static_cast<unsigned long long int>(header.dimensions.width * header.dimensions.height);

  // Array of Structures
  std::vector<Pixel> pixel_data(static_cast<std::size_t>(pixel_count));

  gsl::span<Pixel> pixel_span{pixel_data};
  // Determinar la longitud de cada pixel (2 bytes si max_color > 256; else: 1)
  bool is_16_bit = header.max_color > MAX_COLOR_VALUE8;

  // RELLENAR EL ARRAY OF STRUCTURES CON LOS PIXELES
  get_pixels(infile, pixel_data, pixel_count, is_16_bit);


  // Escribir en outfile, INFO COMMAND
  write_info(outfile, header, pixel_data, is_16_bit);

  std::ofstream cppm_outfile(args[3], std::ios::binary);

  // COMPRESS COMMAND
  write_cppm(cppm_outfile, header, pixel_data);

  infile.close();
  outfile.close();
  cppm_outfile.close();
  return 0;
};
