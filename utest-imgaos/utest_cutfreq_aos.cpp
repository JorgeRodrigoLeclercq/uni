//
// Created by golden on 11/14/24.
//

#include <gtest/gtest.h>
#include <gsl/gsl>
#include <vector>
#include <unordered_map>
#include "../imgaos/cutfreq.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(readability-magic-numbers)

TEST(MenosFrecuentesTest, ColoresMenosFrecuentes) {
  std::unordered_map<Pixel, int> const colores = {
    {Pixel{255, 0, 0}, 5},  // Rojo, 5 veces
    {Pixel{0, 255, 0}, 3},  // Verde, 3 veces
    {Pixel{0, 0, 255}, 1}   // Azul, 1 vez
  };

  BoundedPriorityQueue const resultado = menosFrecuentes(colores, 2);
  const auto& elements = resultado.get_all();

  // Verificar que el tamaño de la cola es correcto
  EXPECT_EQ(elements.size(), 2);
  // Verificar que los colores menos frecuentes están en la cola
  EXPECT_TRUE((elements[1] == Pixel{0, 0, 255} && elements[2] == Pixel{0, 255, 0})
          || (elements[1] == Pixel{0, 255, 0} && elements[2] == Pixel{0, 0, 255}));
  // Verificar que el color más frecuente (Rojo) no está en la cola
  EXPECT_FALSE((elements[1] ==  Pixel{255, 0, 0}) && (elements[2] == Pixel{255, 0, 0}));
}



// NOLINTEND(readability-magic-numbers)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)