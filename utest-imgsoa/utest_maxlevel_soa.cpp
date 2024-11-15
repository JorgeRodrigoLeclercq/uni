#include <gtest/gtest.h>
#include <vector>
#include "../imgsoa/maxlevel.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(readability-magic-numbers)

// Caso base de prueba: Verifica que el valor de max_color se actualiza y los píxeles se escalan correctamente en SoA
TEST(MaxLevelTest, TestLevelUpdateSoA) {
  // Inicializar ImageHeader con max_color a 255
  ImageHeader header{"P6", {100, 100}, 255};  // Imagen de 100x100 con max_color 255
  bool is_16_bit = false;

  // Crear SoA con algunos píxeles para probar
  SoA pixel_data = {
    .r = {100, 50},  // Componente rojo
    .g = {150, 75},  // Componente verde
    .b = {200, 100}  // Componente azul
  };

  // Llamar a la función maxlevel
  maxlevel(500, is_16_bit, pixel_data, header);

  // Verificar que max_color se actualiza correctamente
  EXPECT_EQ(header.max_color, 500);
  EXPECT_TRUE(is_16_bit);  // Debería ser 16 bits

  // Verificar que los valores de los píxeles se han escalado correctamente
  EXPECT_EQ(pixel_data.r[0], 196);
  EXPECT_EQ(pixel_data.g[0], 294);
  EXPECT_EQ(pixel_data.b[0], 392);

  EXPECT_EQ(pixel_data.r[1], 98);
  EXPECT_EQ(pixel_data.g[1], 147);
  EXPECT_EQ(pixel_data.b[1], 196);
}

// Test para verificar si el valor de is_16_bit se ajusta correctamente en SoA
TEST(MaxLevelTest, TestIs16BitSoA) {
  ImageHeader header{"P6", {100, 100}, 255};  // Inicializar con max_color 255
  bool is_16_bit = false;

  // Crear SoA con algunos píxeles para probar
  SoA pixel_data = {
    .r = {100},  // Componente rojo
    .g = {150},  // Componente verde
    .b = {200}   // Componente azul
  };

  // Probar con un nuevo nivel mayor a 255, lo que debería establecer is_16_bit en true
  maxlevel(1000, is_16_bit, pixel_data, header);
  EXPECT_TRUE(is_16_bit);

  // Probar con un nuevo nivel menor o igual a 255, lo que debería mantener is_16_bit como false
  maxlevel(255, is_16_bit, pixel_data, header);
  EXPECT_FALSE(is_16_bit);
}

// Test para verificar la escala de píxeles con 8 bits en SoA
TEST(MaxLevelTest, TestScale8BitSoA) {
  ImageHeader header{"P6", {2, 2}, 255};  // Imagen de 2x2 con max_color 255
  bool is_16_bit = false;

  // Crear SoA con algunos píxeles para probar
  SoA pixel_data = {
    .r = {255, 0},  // Componente rojo
    .g = {255, 0},  // Componente verde
    .b = {255, 0}   // Componente azul
  };

  maxlevel(255, is_16_bit, pixel_data, header);

  // Verificar que los valores de los píxeles no cambian para 8 bits
  EXPECT_EQ(pixel_data.r[0], 255);
  EXPECT_EQ(pixel_data.g[0], 255);
  EXPECT_EQ(pixel_data.b[0], 255);

  EXPECT_EQ(pixel_data.r[1], 0);
  EXPECT_EQ(pixel_data.g[1], 0);
  EXPECT_EQ(pixel_data.b[1], 0);
}

// Test para verificar la escala de píxeles con 16 bits en SoA
TEST(MaxLevelTest, TestScale16BitSoA) {
  ImageHeader header{"P6", {2, 2}, 255};  // Imagen de 2x2 con max_color 255
  bool is_16_bit = false;

  // Crear SoA con algunos píxeles para probar
  SoA pixel_data = {
    .r = {255, 0},  // Componente rojo
    .g = {255, 0},  // Componente verde
    .b = {255, 0}   // Componente azul
  };

  maxlevel(1000, is_16_bit, pixel_data, header);  // Actualizar a 16 bits

  // Verificar que los valores de los píxeles se escalan correctamente para 16 bits
  EXPECT_EQ(pixel_data.r[0], 1000);
  EXPECT_EQ(pixel_data.g[0], 1000);
  EXPECT_EQ(pixel_data.b[0], 1000);

  EXPECT_EQ(pixel_data.r[1], 0);
  EXPECT_EQ(pixel_data.g[1], 0);
  EXPECT_EQ(pixel_data.b[1], 0);
}

// Test para verificar el cambio de representación de píxeles (3 bytes a 6 bytes y viceversa) en SoA
TEST(MaxLevelTest, TestChangeRepresentationSoA) {
  // Imagen con max_color 255 (representación 3 bytes por píxel)
  ImageHeader header{"P6", {2, 2}, 255};  // Imagen 2x2 con 255 como valor máximo
  bool is_16_bit = false;  // Inicialmente no es de 16 bits

  // Crear SoA con algunos píxeles para probar
  SoA pixel_data = {
    .r = {255, 0},  // Componente rojo
    .g = {255, 0},  // Componente verde
    .b = {255, 0}   // Componente azul
  };

  // Escalar a 16 bits (max_color > 255)
  maxlevel(1000, is_16_bit, pixel_data, header);

  // Verificar que se haya cambiado a 16 bits (6 bytes por píxel)
  EXPECT_EQ(header.max_color, 1000);
  EXPECT_TRUE(is_16_bit);  // La imagen ahora es de 16 bits

  // Verificar que los valores de los píxeles se han escalado correctamente para 16 bits
  EXPECT_EQ(pixel_data.r[0], 1000);
  EXPECT_EQ(pixel_data.g[0], 1000);
  EXPECT_EQ(pixel_data.b[0], 1000);

  // Escalar de nuevo a 8 bits (max_color <= 255)
  maxlevel(255, is_16_bit, pixel_data, header);

  // Verificar que se ha vuelto a 8 bits (3 bytes por píxel)
  EXPECT_EQ(header.max_color, 255);
  EXPECT_FALSE(is_16_bit);  // La imagen ahora es de 8 bits

  // Verificar que los valores de los píxeles no se desborden al escalar de vuelta a 8 bits
  EXPECT_EQ(pixel_data.r[0], 255);
  EXPECT_EQ(pixel_data.g[0], 255);
  EXPECT_EQ(pixel_data.b[0], 255);

  EXPECT_EQ(pixel_data.r[1], 0);
  EXPECT_EQ(pixel_data.g[1], 0);
  EXPECT_EQ(pixel_data.b[1], 0);
}

TEST(MaxLevelTest, TestExtremeColorValuesSoA) {
  ImageHeader header{"P6", {2, 2}, 255};  // Imagen 2x2 con max_color 255
  bool is_16_bit = false;

  // Crear SoA con algunos píxeles para probar
  SoA pixel_data = {
    .r = {255, 0},  // Componente rojo
    .g = {255, 0},  // Componente verde
    .b = {255, 0}   // Componente azul
  };

  // Escalar a un max_color mayor que 255
  maxlevel(1000, is_16_bit, pixel_data, header);

  // Verificar que los valores no excedan el nuevo max_color
  EXPECT_EQ(pixel_data.r[0], 1000);
  EXPECT_EQ(pixel_data.g[0], 1000);
  EXPECT_EQ(pixel_data.b[0], 1000);

  // Escalar nuevamente a un max_color más pequeño, como 100
  maxlevel(100, is_16_bit, pixel_data, header);

  // Verificar que los valores de los píxeles no excedan el límite de 100
  EXPECT_EQ(pixel_data.r[0], 100);  // El valor original era 255, pero ahora se debe reducir
  EXPECT_EQ(pixel_data.g[0], 100);
  EXPECT_EQ(pixel_data.b[0], 100);

  // Verificar que los valores del segundo píxel permanecen dentro de los límites
  EXPECT_EQ(pixel_data.r[1], 0);
  EXPECT_EQ(pixel_data.g[1], 0);
  EXPECT_EQ(pixel_data.b[1], 0);
}

// NOLINTEND(readability-magic-numbers)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)



