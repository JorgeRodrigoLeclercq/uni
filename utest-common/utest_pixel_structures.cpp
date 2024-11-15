//
// Created by golden on 11/15/24.
//
#include <gtest/gtest.h>
#include "../common/pixel_structures.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(readability-magic-numbers)

TEST(PixelTest, ConstructorTest) {
  // Prueba con valores predeterminados
  Pixel const pixel1;
  EXPECT_EQ(pixel1.channels.red, 0);
  EXPECT_EQ(pixel1.channels.green, 0);
  EXPECT_EQ(pixel1.channels.blue, 0);

  // Prueba con valores personalizados
  Pixel const pixel2(255, 128, 64);
  EXPECT_EQ(pixel2.channels.red, 255);
  EXPECT_EQ(pixel2.channels.green, 128);
  EXPECT_EQ(pixel2.channels.blue, 64);
}

TEST(PixelTest, ComparisonOperators) {
  Pixel const pixel1(255, 0, 0);  // Rojo
  Pixel const pixel2(255, 0, 0);  // Rojo
  Pixel const pixel3(0, 255, 0);  // Verde
  Pixel const pixel4(0, 0, 255);  // Azul
  Pixel const pixel5(0, 0, 0);  // Negro

  // Verificar que los operadores de igualdad funcionan
  EXPECT_TRUE(pixel1 == pixel2);  // Los dos píxeles son iguales
  EXPECT_FALSE(pixel1 == pixel3); // Los píxeles no son iguales

  // Verificar que el operador < funciona
  EXPECT_TRUE(pixel4 < pixel5);   // Se mira primero el azul
  EXPECT_TRUE(pixel3 < pixel5);   // Ante empate de azul, se mira el valor verde
  EXPECT_TRUE(pixel2 < pixel5);  // Ante empate de azul y verde, se mira el valor rojo
}

#include <unordered_set>

TEST(PixelTest, HashTest) {
  Pixel const pixel1(255, 0, 0);
  Pixel const pixel2(255, 0, 0);
  Pixel const pixel3(0, 255, 0);

  // Usar std::unordered_set para verificar que los píxeles con los mismos valores tienen el mismo hash
  std::unordered_set<Pixel> pixel_set;
  pixel_set.insert(pixel1);
  pixel_set.insert(pixel2);
  pixel_set.insert(pixel3);

  EXPECT_EQ(pixel_set.size(), 2); // pixel1 y pixel2 deberían tener el mismo hash, pixel3 es diferente
}

TEST(SoATest, ResizeTest) {
  SoA soa;

  // Asegurarse de que las longitudes iniciales son 0
  EXPECT_EQ(soa.r.size(), 0);
  EXPECT_EQ(soa.g.size(), 0);
  EXPECT_EQ(soa.b.size(), 0);

  // Redimensionar
  soa.resize(5);

  // Asegurarse de que las longitudes son correctas
  EXPECT_EQ(soa.r.size(), 5);
  EXPECT_EQ(soa.g.size(), 5);
  EXPECT_EQ(soa.b.size(), 5);

  // Verificar que los valores iniciales en los arrays sean 0
  EXPECT_EQ(soa.r[0], 0);
  EXPECT_EQ(soa.g[0], 0);
  EXPECT_EQ(soa.b[0], 0);
}

// NOLINTEND(readability-magic-numbers)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)