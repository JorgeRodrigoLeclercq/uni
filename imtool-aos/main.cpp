#include <iostream>
#include <fstream>
#include "functions.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::ifstream infile(argv[1], std::ios::binary);  // abrir en modo binario
    if (!infile) {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    // Variables del header
    std::string magic_number;
    int width, height, max_color;

    // INFO COMMAND
    get_header(infile, magic_number, width, height, max_color);

    // Tamaño en pixeles de la imagen
    int pixel_count = width * height;

    // Array of Structures
    std::vector<Pixel> pixel_data(pixel_count);

    // Determinar la longitud de cada pixel (2 bytes si max_color > 256; else: 1)
    bool is_16_bit = max_color > 255;

    // RELLENAR EL ARRAY OF STRUCTURES CON LOS PIXELES
    get_pixels(infile, pixel_data, pixel_count, is_16_bit);

    infile.close();
    return 0;
}
