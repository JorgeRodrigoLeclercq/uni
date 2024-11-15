#include "cmp.hpp"
#include "binaryio.hpp"  // Para leer imágenes binarias
#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>

// Comparar los componentes RGB de dos píxeles con tolerancia de ±tolerancia
bool compare_pixels(const Pixel& pixel1, const Pixel& pixel2, int tolerance) {
  return (std::abs(pixel1.channels.red - pixel2.channels.red) <= tolerance) &&
         (std::abs(pixel1.channels.green - pixel2.channels.green) <= tolerance) &&
         (std::abs(pixel1.channels.blue - pixel2.channels.blue) <= tolerance);
}

// Función para comparar imágenes generadas y esperadas
bool compare_images(const std::string& generated_file, int tolerance, const std::string& expected_file) {
  std::vector<Pixel> gen_pixels;
  std::vector<Pixel> exp_pixels;

  // Abrir el archivo esperado y el archivo generado
  std::ifstream exp_file(expected_file, std::ios::binary);
  std::ifstream gen_file(generated_file, std::ios::binary);

  if (!gen_file.is_open() || !exp_file.is_open()) {
    std::cerr << "Error al abrir los archivos." << '\n';
    return false;
  }

  // Leer los píxeles de ambos archivos utilizando read_binary de binaryio.hpp
  while (exp_file) {
    auto pixel = read_binary<Pixel>(exp_file);
    exp_pixels.push_back(pixel);
  }
  while (gen_file) {
    auto pixel = read_binary<Pixel>(gen_file);
    gen_pixels.push_back(pixel);
  }

  // Verificar que ambas imágenes tienen el mismo tamaño
  if (gen_pixels.size() != exp_pixels.size()) {
    std::cerr << "Las imágenes no tienen el mismo número de píxeles." << '\n';
    return false;
  }

  // Comparar píxel por píxel
  for (size_t i = 0; i < gen_pixels.size(); ++i) {
    if (!compare_pixels(gen_pixels[i], exp_pixels[i], tolerance)) {
      std::cerr << "FAIL: Píxel en posición " << i << " no coincide con la tolerancia." << '\n';
      return false;
    }
  }

  return true;  // Las imágenes coinciden dentro de la tolerancia
}



