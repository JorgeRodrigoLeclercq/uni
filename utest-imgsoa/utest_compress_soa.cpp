#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <tuple>
#include "../imgsoa/compress.hpp"
#include "../common/pixel_structures.hpp"

// Test para la función compress con verificación del encabezado
TEST(CompressTests, BasicCompressFunction_SoA) {
  constexpr int MAX_8_BIT = 255;
  ImageHeader header;
  header.dimensions.width = 2;
  header.dimensions.height = 2;
  header.max_color = MAX_8_BIT;

  SoA pixel_data;
  pixel_data.resize(4);
  pixel_data.r = {MAX_8_BIT, 0, 0, MAX_8_BIT};
  pixel_data.g = {0, MAX_8_BIT, 0, MAX_8_BIT};
  pixel_data.b = {0, 0, MAX_8_BIT, MAX_8_BIT};

  std::string const temp_filename = "temp_cppm_output_soa.cppm";
  std::ofstream cppm_outfile(temp_filename, std::ios::binary);
  ASSERT_TRUE(cppm_outfile.is_open());

  // Actuar: Comprimir y escribir en el archivo temporal
  compress(cppm_outfile, header, pixel_data);
  cppm_outfile.close();

  // Verificar: Comprobar si el archivo comienza con "C6"
  std::ifstream result_file(temp_filename, std::ios::binary);
  ASSERT_TRUE(result_file.is_open());

  // Leer los primeros dos bytes y verificar que coincidan con "C6"
  std::array<char, 2> magic_number{};
  result_file.read(magic_number.data(), 2);
  ASSERT_EQ(magic_number[0], 'C');
  ASSERT_EQ(magic_number[1], '6');

  // Comprobar si el archivo tiene contenido más allá del encabezado
  result_file.seekg(0, std::ios::end);
  auto file_size = result_file.tellg();
  ASSERT_GT(file_size, 2); // Asegurarse de que el archivo tiene más que solo el encabezado

  // Limpiar
  result_file.close();
  //int const result = std::remove(temp_filename.c_str());
}


// Test para la función write_color_table con un tamaño de tabla uint8_t
TEST(WriteColorTableTests, SmallColorTable_Uint8_SoA) {
  // Usar un archivo temporal para probar la salida
  std::string const temp_filename = "temp_output_soa.cppm";
  std::ofstream outfile(temp_filename, std::ios::binary);
  ASSERT_TRUE(outfile.is_open());

  // Inicializar los datos de píxeles con la estructura SoA
  SoA pixel_data;
  constexpr int MAX_8_BIT = 255;
  pixel_data.resize(2);
  pixel_data.r = {MAX_8_BIT, 0};
  pixel_data.g = {0, MAX_8_BIT};
  pixel_data.b = {0, 0};

  // Crear la tabla de colores con claves de píxel y valores enteros
  std::map<std::tuple<uint8_t, uint8_t, uint8_t>, int> const color_table = {
    {std::make_tuple(MAX_8_BIT, 0, 0), 0},
    {std::make_tuple(0, MAX_8_BIT, 0), 1}
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
  ASSERT_EQ(output.size(), 2); // Esperamos 1 byte por píxel

  // Limpiar
  result_file.close();
  //int const result = std::remove(temp_filename.c_str());
}

// Test para la función write_color_table con un tamaño de tabla uint16_t
TEST(WriteColorTableTests, MediumColorTable_Uint16_SoA) {
  // Usar un archivo temporal para la salida
  std::string const temp_filename = "temp_output_medium_soa.cppm";
  std::ofstream outfile(temp_filename, std::ios::binary);
  ASSERT_TRUE(outfile.is_open());

  // Inicializar los datos de píxeles con la estructura SoA
  SoA pixel_data;
  constexpr int MAX_16_BIT = 65535;
  pixel_data.resize(4);
  pixel_data.r = {MAX_16_BIT, 0, 0, MAX_16_BIT};
  pixel_data.g = {0, MAX_16_BIT, 0, MAX_16_BIT};
  pixel_data.b = {0, 0, MAX_16_BIT, MAX_16_BIT};

  // Crear la tabla de colores con claves de píxel y valores enteros
  std::map<std::tuple<uint8_t, uint8_t, uint8_t>, int> color_table;
  constexpr int THREE_HUNDRED = 300;
  for (int i = 0; i < THREE_HUNDRED; ++i) {
    color_table[std::make_tuple(i, i, i)] = i;
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
  //const int result = std::remove(temp_filename.c_str());
}

// Test para la función write_color_table con un tamaño de tabla uint32_t
TEST(WriteColorTableTests, LargeColorTable_Uint32_SoA) {
  // Usar un archivo temporal para la salida
  std::string const temp_filename = "temp_output_large_soa.cppm";
  std::ofstream outfile(temp_filename, std::ios::binary);
  ASSERT_TRUE(outfile.is_open());

  // Inicializar los datos de píxeles con la estructura SoA
  SoA pixel_data;
  constexpr int MAX_8_BIT = 255;
  pixel_data.resize(2);
  pixel_data.r = {MAX_8_BIT, MAX_8_BIT};
  pixel_data.g = {MAX_8_BIT, 0};
  pixel_data.b = {0, MAX_8_BIT};

  constexpr int SEVENTY_THOUSAND = 70000;
  // Crear la tabla de colores con claves de píxel y valores enteros
  std::map<std::tuple<uint8_t, uint8_t, uint8_t>, int> color_table;
  for (int i = 0; i < SEVENTY_THOUSAND; ++i) {
    constexpr int TWO_FIFTY_SIX = 256;
    color_table[std::make_tuple(i % TWO_FIFTY_SIX, i % TWO_FIFTY_SIX, i % TWO_FIFTY_SIX)] = i;
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
  //const int result = std::remove(temp_filename.c_str());
}


// Test para la función write_color_table con una tabla de colores de tamaño excesivo (caso de error)
TEST(WriteColorTableTests, OversizedColorTable_Error_SoA) {
  // Usar un archivo temporal para la salida
  std::string const temp_filename = "temp_output_error_soa.cppm";
  std::ofstream outfile(temp_filename, std::ios::binary);
  ASSERT_TRUE(outfile.is_open());

  // Inicializar los datos de píxeles con la estructura SoA
  SoA pixel_data;
  constexpr int MAX_8_BIT = 255;
  pixel_data.resize(1);
  pixel_data.r = {MAX_8_BIT};
  pixel_data.g = {0};
  pixel_data.b = {0};

  constexpr int HUNDRED = 100;
  // Crear una tabla de colores falsa de tamaño excesivo con claves de píxel y valores enteros
  std::map<std::tuple<uint8_t, uint8_t, uint8_t>, int> color_table;
  for (int i = 0; i <= HUNDRED; ++i) {
    color_table[std::make_tuple(i, i, i)] = i;
  }

  // Llamar a la función con el flujo de archivo
  write_color_table(outfile, pixel_data, color_table);

  // Cerrar el archivo para completar el proceso de escritura
  outfile.close();

  // Leer el archivo para verificar su contenido
  std::ifstream result_file(temp_filename, std::ios::binary);
  ASSERT_TRUE(result_file.is_open());

  std::string const output((std::istreambuf_iterator<char>(result_file)), std::istreambuf_iterator<char>());

  // Dado que estamos usando uint8_t, esperamos que haya un error si la tabla es demasiado grande
  ASSERT_EQ(output.size(), 0);

  // Limpiar
  result_file.close();
  //const int result = std::remove(temp_filename.c_str());
}
