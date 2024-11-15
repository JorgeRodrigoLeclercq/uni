#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "imgaos/compress.hpp"
#include "../common/pixel_structures.hpp"

// Test para la función de compresión con verificación de encabezado
TEST(CompressTests, BasicCompressFunction) {
  constexpr int MAX_8_BIT = 255;
  ImageHeader header;
  header.dimensions.width = 2;
  header.dimensions.height = 2;
  header.max_color = MAX_8_BIT;

  std::vector<Pixel> const pixel_data = {
    Pixel(255, 0, 0), Pixel(0, 255, 0),
    Pixel(0, 0, 255), Pixel(255, 255, 255)
  };

  std::string const temp_filename = "temp_cppm_output.cppm";
  std::ofstream cppm_outfile(temp_filename, std::ios::binary);
  ASSERT_TRUE(cppm_outfile.is_open());

  // Actuar: Comprimir y escribir en un archivo temporal
  compress(cppm_outfile, header, pixel_data);
  cppm_outfile.close();

  // Assert: Verificar que el archivo comienza con "C6"
  std::ifstream result_file(temp_filename, std::ios::binary);
  ASSERT_TRUE(result_file.is_open());

  // Leer los dos primeros bytes y verificar que coincidan con "C6"
  std::array<char, 2> magic_number{};
  result_file.read(magic_number.data(), 2);
  ASSERT_EQ(magic_number[0], 'C');
  ASSERT_EQ(magic_number[1], '6');

  // Verificar que el archivo tenga contenido más allá del encabezado
  result_file.seekg(0, std::ios::end);
  auto file_size = result_file.tellg();
  ASSERT_GT(file_size, 2); // Asegurarse de que el archivo tiene más que solo el encabezado

  // Limpiar
  result_file.close();
  int const result = std::remove(temp_filename.c_str());
}


// Test para la función write_color_table con tamaño de tabla uint8_t
TEST(WriteColorTableTests, SmallColorTable_Uint8) {
  // Usar un archivo temporal para probar la salida
  std::string const temp_filename = "temp_output.cppm";
  std::ofstream outfile(temp_filename, std::ios::binary);
  ASSERT_TRUE(outfile.is_open());

  // Inicializar los datos de píxeles y la tabla de colores con constructores explícitos de Pixel
  std::vector<Pixel> const pixel_data = {
    Pixel(255, 0, 0), Pixel(0, 255, 0)
  };

  std::map<Pixel, int> const color_table = {
    {Pixel(255, 0, 0), 0},
    {Pixel(0, 255, 0), 1}
  };

  // Llamar a la función con el flujo de archivo
  write_color_table(outfile, pixel_data, color_table);

  // Cerrar el archivo para completar el proceso de escritura
  outfile.close();

  // Leer el archivo para verificar su contenido
  std::ifstream result_file(temp_filename, std::ios::binary);
  ASSERT_TRUE(result_file.is_open());

  // Verificar que el tamaño de la salida coincida con el tamaño esperado (1 byte por píxel para uint8_t)
  std::string const output((std::istreambuf_iterator<char>(result_file)), std::istreambuf_iterator<char>());
  ASSERT_EQ(output.size(), 2); // Se espera 1 byte por píxel

  // Limpiar
  result_file.close();
  int const result = std::remove(temp_filename.c_str());
}

// Test para la función write_color_table con tamaño de tabla uint16_t
TEST(WriteColorTableTests, MediumColorTable_Uint16) {
  // Usar un archivo temporal para la salida
  std::string const temp_filename = "temp_output_medium.cppm";
  std::ofstream outfile(temp_filename, std::ios::binary);
  ASSERT_TRUE(outfile.is_open());

  // Inicializar los datos de píxeles con constructores explícitos de Pixel
  std::vector<Pixel> const pixel_data = {
    Pixel(65535, 0, 0), Pixel(0, 65535, 0),
    Pixel(0, 0, 65535), Pixel(65535, 65535, 65535)
};

  // Crear la tabla de colores con claves de Pixel y valores enteros
  std::map<Pixel, int> color_table;
  constexpr int THREE_HUNDRED = 300;
  for (int i = 0; i < THREE_HUNDRED; ++i) {
    color_table[Pixel(i, i, i)] = i;
  }

  // Llamar a la función con el flujo de archivo
  write_color_table(outfile, pixel_data, color_table);

  // Cerrar el archivo para completar el proceso de escritura
  outfile.close();

  // Leer el archivo para verificar su contenido
  std::ifstream result_file(temp_filename, std::ios::binary);
  ASSERT_TRUE(result_file.is_open());

  std::string const output((std::istreambuf_iterator<char>(result_file)), std::istreambuf_iterator<char>());

  // Dado que estamos usando uint16_t (2 bytes por píxel), esperamos 8 bytes (4 píxeles * 2 bytes)
  ASSERT_EQ(output.size(), 8);

  // Limpiar
  result_file.close();
  const int result = std::remove(temp_filename.c_str());
}

// Test para la función write_color_table con tamaño de tabla uint32_t
TEST(WriteColorTableTests, LargeColorTable_Uint32) {
  // Usar un archivo temporal para la salida
  std::string const temp_filename = "temp_output_large.cppm";
  std::ofstream outfile(temp_filename, std::ios::binary);
  ASSERT_TRUE(outfile.is_open());

  // Inicializar los datos de píxeles con constructores explícitos de Pixel
  std::vector<Pixel> const pixel_data = {
    Pixel(255, 255, 0), Pixel(255, 0, 255)
};

  constexpr int SEVENTY_THOUSAND = 70000;
  // Crear la tabla de colores con claves de Pixel y valores enteros
  std::map<Pixel, int> color_table;
  for (int i = 0; i < SEVENTY_THOUSAND; ++i) {
    constexpr int TWO_FIFTY_SIX = 256;
    color_table[Pixel(i % TWO_FIFTY_SIX, i % TWO_FIFTY_SIX, i % TWO_FIFTY_SIX)] = i;
  }

  // Llamar a la función con el flujo de archivo
  write_color_table(outfile, pixel_data, color_table);

  // Cerrar el archivo para completar el proceso de escritura
  outfile.close();

  // Leer el archivo para verificar su contenido
  std::ifstream result_file(temp_filename, std::ios::binary);
  ASSERT_TRUE(result_file.is_open());

  std::string const output((std::istreambuf_iterator<char>(result_file)), std::istreambuf_iterator<char>());

  // Dado que estamos usando uint32_t (4 bytes por píxel), esperamos 8 bytes (2 píxeles * 4 bytes)
  ASSERT_EQ(output.size(), 8);

  // Limpiar
  result_file.close();
  const int result = std::remove(temp_filename.c_str());
}


// Test para la función write_color_table con una tabla demasiado grande (caso de error)
TEST(WriteColorTableTests, OversizedColorTable_Error) {
  // Usar un archivo temporal para la salida
  std::string const temp_filename = "temp_output_error.cppm";
  std::ofstream outfile(temp_filename, std::ios::binary);
  ASSERT_TRUE(outfile.is_open());

  // Inicializar los datos de píxeles con constructores explícitos de Pixel
  std::vector<Pixel> const pixel_data = {
    Pixel(255, 0, 0)
};

  constexpr int HUNDRED = 100;
  // Crear una tabla de colores demasiado grande simulada con claves de Pixel y valores enteros
  std::map<Pixel, int> color_table;
  // Cambiar el tipo de i a int
  for (int i = 0; i <= HUNDRED; ++i) { // Límite simulado para la prueba
    constexpr int TWO_FIFTY_SIX = 256;
    color_table[Pixel(i % TWO_FIFTY_SIX, i % TWO_FIFTY_SIX, i % TWO_FIFTY_SIX)] = i;
  }

  // Esperar que la función write_color_table termine con código 1 debido a la tabla demasiado grande
  ASSERT_EXIT(write_color_table(outfile, pixel_data, color_table),
              ::testing::ExitedWithCode(1), "Error: Color table too large.");

  // Limpiar
  outfile.close();
  const int result = std::remove(temp_filename.c_str());
}
