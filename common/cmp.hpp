#ifndef CMP_HPP
#define CMP_HPP

#include <string>
#include "pixel_structures.hpp"

constexpr int DEFAULT_TOLERANCE = 5;
// Función para comparar dos píxeles con tolerancia de ±tolerancia
bool compare_pixels(const Pixel& pixel1, const Pixel& pixel2, int tolerance = DEFAULT_TOLERANCE);

// Función para comparar dos imágenes
bool compare_images(const std::string& generated_file, int tolerance = DEFAULT_TOLERANCE  , const std::string& expected_file = "");

#endif // CMP_HPP


