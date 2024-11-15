#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include "../common/header.hpp"

// Función para crear un archivo temporal PPM con un header específico
namespace {
  void create_temp_ppm_with_header(const std::string& magic_number, ImageDimensions const dimensions, int max_color, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    file << magic_number << "\n" << dimensions.width << " " << dimensions.height << "\n" << max_color << "\n";
    file.close();
  }
}

// Test para verificar que el header se lee correctamente de un archivo PPM
TEST(GetHeaderTest, ReadsHeaderCorrectly) {
  const std::string filename = "test_image.ppm";

  constexpr int MAX_8_BIT = 255;
  // Crear un archivo PPM de prueba
  constexpr ImageDimensions dimensions(2, 2);
  create_temp_ppm_with_header("P6", dimensions, MAX_8_BIT, filename);

  std::ifstream infile(filename, std::ios::binary);
  ImageHeader header;
  get_header(infile, header);

  // Verificar que los valores del header son correctos
  EXPECT_EQ(header.magic_number, "P6");
  EXPECT_EQ(header.dimensions.width, 2);
  EXPECT_EQ(header.dimensions.height, 2);
  EXPECT_EQ(header.max_color, 255);

  infile.close();
  const int result = std::remove(filename.c_str());
}