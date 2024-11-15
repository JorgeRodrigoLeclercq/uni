#include <gtest/gtest.h>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include "../common/progargs.hpp""

// Test para verificar que la función checkInfoAndCompress maneja correctamente el número de argumentos
TEST(CheckInfoAndCompressTest, HandlesTooManyArguments) {
  // Redirigir std::cerr para capturar el mensaje de error
  std::ostringstream const err_output;
  std::streambuf* original_cerr_buffer = std::cerr.rdbuf(err_output.rdbuf());

  // Test con 5 argumentos (más de los 4 permitidos)
  EXPECT_EXIT(checkInfoAndCompress(5), testing::ExitedWithCode(-1), "Error: Invalid number of arguments: 4");

  // Test con 6 argumentos (más de los 4 permitidos)
  EXPECT_EXIT(checkInfoAndCompress(6), testing::ExitedWithCode(-1), "Error: Invalid number of arguments: 5");

  // Restaurar el buffer original de std::cerr
  std::cerr.rdbuf(original_cerr_buffer);
}

// Test para verificar que la función checkInfoAndCompress no falla con el número correcto de argumentos
TEST(CheckInfoAndCompressTest, DoesNotExitWithValidArguments) {
  // No debe salir si el número de argumentos es 4 o menos
  EXPECT_EQ(checkInfoAndCompress(4), 0);
  EXPECT_EQ(checkInfoAndCompress(3), 0);
  EXPECT_EQ(checkInfoAndCompress(2), 0);
}
