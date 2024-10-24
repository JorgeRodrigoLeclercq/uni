#include <iostream>
#include <fstream>
//#include <gsl/gsl>
#include "functions.hpp"

int main(int argc, const char *argv[]) {
    //gsl::span<const char *> const args{argv, gsl::narrow<std::size_t>(argc)}; // Debe ser así por reglas de codificación
//    if (argc < 2) {
//        std::cerr << "Usage: " << args[0] << " <filename>" << "\n";
//        return 1;
//    }

    std::ifstream infile(argv[1], std::ios::binary);  // abrir en modo binario
//    if (!infile) {
//        std::cerr << "Error: Could not open file " << args[1] << "\n";
//        return 1;
//    }

    std::ofstream outfile(argv[2], std::ios::binary);

    // Variables del header
    std::string magic_number;
    int width, height, max_color;

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

    // Escribir en outfile
    write_info(outfile, magic_number, width, height, max_color, pixel_data, is_16_bit);

    infile.close();
    outfile.close();
    return 0;
}
