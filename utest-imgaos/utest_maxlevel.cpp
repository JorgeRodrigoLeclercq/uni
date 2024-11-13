#include <gtest/gtest.h>
#include "../imgaos/maxlevel.hpp"
#include "../common/pixel_structures.hpp"
#include "../common/header.hpp"  // Asegúrate de incluir la cabecera de ImageHeader

// Caso base de prueba: Verifica que el valor de max_color se actualiza y los píxeles se escalan correctamente
TEST(MaxLevelTest, TestLevelUpdate) {
  // Inicializar ImageHeader con max_color a 255
  ImageHeader header{"P6", {100, 100}, 255};  // Imagen de 100x100 con max_color 255
  bool is_16_bit = false;

  // Crear algunos píxeles para probar
  std::vector<Pixel> pixel_data = {
    Pixel(100, 150, 200),
    Pixel(50, 75, 100)
  };

  // Llamar a la función maxlevel
  maxlevel(500, is_16_bit, pixel_data, header);

  // Verificar que max_color se actualizó correctamente
  EXPECT_EQ(header.max_color, 500);
  EXPECT_TRUE(is_16_bit);  // Debería ser 16 bits

  // Verificar que los valores de los píxeles se han escalado correctamente
  EXPECT_EQ(pixel_data[0].channels.red, 200);
  EXPECT_EQ(pixel_data[0].channels.green, 300);
  EXPECT_EQ(pixel_data[0].channels.blue, 400);

  EXPECT_EQ(pixel_data[1].channels.red, 100);
  EXPECT_EQ(pixel_data[1].channels.green, 150);
  EXPECT_EQ(pixel_data[1].channels.blue, 200);
}

// Test para verificar si el valor de is_16_bit se ajusta correctamente
TEST(MaxLevelTest, TestIs16Bit) {
  ImageHeader header{"P6", {100, 100}, 255};  // Inicializar con max_color 255
  bool is_16_bit = false;

  std::vector<Pixel> pixel_data = {
    Pixel(100, 150, 200)
  };

  // Probar con un nuevo nivel mayor a 255, lo que debería establecer is_16_bit en true
  maxlevel(1000, is_16_bit, pixel_data, header);
  EXPECT_TRUE(is_16_bit);

  // Probar con un nuevo nivel menor o igual a 255, lo que debería mantener is_16_bit en false
  maxlevel(255, is_16_bit, pixel_data, header);
  EXPECT_FALSE(is_16_bit);
}

// Test para verificar la escala de píxeles con 8 bits
TEST(MaxLevelTest, TestScale8Bit) {
  ImageHeader header{"P6", {2, 2}, 255};  // Imagen de 2x2 con max_color 255
  bool is_16_bit = false;

  std::vector<Pixel> pixel_data = {
    Pixel(255, 255, 255),  // Píxel con color máximo
    Pixel(0, 0, 0)         // Píxel con color mínimo
  };

  maxlevel(255, is_16_bit, pixel_data, header);

  // Verificar que los valores de los píxeles no cambian para 8 bits
  EXPECT_EQ(pixel_data[0].channels.red, 255);
  EXPECT_EQ(pixel_data[0].channels.green, 255);
  EXPECT_EQ(pixel_data[0].channels.blue, 255);

  EXPECT_EQ(pixel_data[1].channels.red, 0);
  EXPECT_EQ(pixel_data[1].channels.green, 0);
  EXPECT_EQ(pixel_data[1].channels.blue, 0);
}

// Test para verificar la escala de píxeles con 16 bits
TEST(MaxLevelTest, TestScale16Bit) {
  ImageHeader header{"P6", {2, 2}, 255};  // Imagen de 2x2 con max_color 255
  bool is_16_bit = false;

  std::vector<Pixel> pixel_data = {
    Pixel(255, 255, 255),  // Píxel con color máximo
    Pixel(0, 0, 0)         // Píxel con color mínimo
  };

  maxlevel(1000, is_16_bit, pixel_data, header);  // Actualizar a 16 bits

  // Verificar que los valores de los píxeles se escalan correctamente para 16 bits
  EXPECT_EQ(pixel_data[0].channels.red, 1000);
  EXPECT_EQ(pixel_data[0].channels.green, 1000);
  EXPECT_EQ(pixel_data[0].channels.blue, 1000);

  EXPECT_EQ(pixel_data[1].channels.red, 0);
  EXPECT_EQ(pixel_data[1].channels.green, 0);
  EXPECT_EQ(pixel_data[1].channels.blue, 0);
}


