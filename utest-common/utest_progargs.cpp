#include <gtest/gtest.h>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include "../common/progargs.hpp"
#include <gsl/gsl>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(readability-magic-numbers)

// Test para comprobar que la función maneja correctamente los casos con número válido de argumentos
TEST(CheckNumberArgsTest, ArgumentCount) {
  // Caso válido: El número de argumentos es 4 o más
  EXPECT_EQ(checkNumberArgs(4), 0);
  EXPECT_EQ(checkNumberArgs(5), 0);

  // Caso inválido: El número de argumentos es menor a 4
  EXPECT_EXIT(checkNumberArgs(3), ::testing::ExitedWithCode(1), "Error: Invalid number of arguments: 2\n");
  EXPECT_EXIT(checkNumberArgs(2), ::testing::ExitedWithCode(1), "Error: Invalid number of arguments: 1\n");
  EXPECT_EXIT(checkNumberArgs(1), ::testing::ExitedWithCode(1), "Error: Invalid number of arguments: 0\n");
}

// Test para verificar que la función checkInfoAndCompress maneja correctamente el número de argumentos
TEST(CheckInfoAndCompressTest, HandlesTooManyArguments) {
  // Redirigir std::cerr para capturar el mensaje de error
  std::ostringstream const err_output;
  std::streambuf* original_cerr_buffer = std::cerr.rdbuf(err_output.rdbuf());

  // Test con 5 argumentos (más de los 4 permitidos)
  EXPECT_EXIT(checkInfoAndCompress(5), testing::ExitedWithCode(255), "Error: Invalid number of arguments: 4");

  // Test con 6 argumentos (más de los 4 permitidos)
  EXPECT_EXIT(checkInfoAndCompress(6), testing::ExitedWithCode(255), "Error: Invalid number of arguments: 5");

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

namespace {
  // Convierte un std::array<std::string, N> en un std::array<char const *, N>, necesario para las pruebas de cutfreq
  template <size_t N>
  std::array<char const *, N> toCArray(const std::array<std::string, N>& input) {
    std::array<char const *, N> c_array{};
    auto iterator = c_array.begin(); // Iterador para llenar el c_array
    for (const auto& str : input) {
      *iterator++ = str.c_str(); // Llenar c_array usando iterador
    }
    return c_array;
  }

}

// Test para verificar que la función checkCutfreq maneja correctamente el número de argumentos
TEST(CheckCutfreqTest, cutfreqNumArgs) {
  // Caso válido: número de argumentos correcto
  std::array<std::string, 5> const valid_args = {"program", "arg1", "arg2", "arg3", "10"};
  auto c_args = toCArray(valid_args);
  EXPECT_EQ(checkCutFreq(c_args, 5), 10);

  // Caso inválido: número de argumentos menor al esperado
  std::array<std::string, 4> const fewer_args = {"program", "arg1", "arg2", "arg3"};
  auto c_fewer_args = toCArray(fewer_args);
  EXPECT_EXIT(checkCutFreq(c_fewer_args, 4), ::testing::ExitedWithCode(1),
              "Error: Invalid number of arguments for cutfreq: 3");

  // Caso inválido: número de argumentos mayor al esperado
  std::array<std::string, 6> const extra_args = {"program", "arg1", "arg2", "arg3", "10", "extra"};
  auto c_extra_args = toCArray(extra_args);
  EXPECT_EXIT(checkCutFreq(c_extra_args, 6), ::testing::ExitedWithCode(1),
              "Error: Invalid number of arguments for cutfreq: 5");
}

TEST(CheckCutfreqTest, cutfreqValidArgs) {
    // Caso inválido: args[4] no es un número
  std::array<std::string, 5> const invalid_cutfreq = {"program", "arg1", "arg2", "arg3", "not_a_number"};
  auto c_invalid_cutfreq = toCArray(invalid_cutfreq);

  EXPECT_EXIT(checkCutFreq(c_invalid_cutfreq, 5), ::testing::ExitedWithCode(1),
              "Error: Invalid cutfreq: not_a_number");

  // Caso inválido: args[4] es un número negativo
  std::array<std::string, 5> const negative_cutfreq = {"program", "arg1", "arg2", "arg3", "-10"};
  auto c_negative_cutfreq = toCArray(negative_cutfreq);

  EXPECT_EXIT(
    checkCutFreq(c_negative_cutfreq, 5), ::testing::ExitedWithCode(1),
              "Error: cutfreq must be a positive number, but got: -10"
  );
}
// NOLINTEND(readability-magic-numbers)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)