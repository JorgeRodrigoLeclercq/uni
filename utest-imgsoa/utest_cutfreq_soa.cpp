//
// Created by golden on 11/15/24.
//
#include <gtest/gtest.h>
#include <gsl/gsl>
#include <vector>
#include <unordered_map>
#include "../imgsoa/cutfreq.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(readability-magic-numbers)

/* contarFrecuencias unittests */
TEST(ContarFrecuenciasTest, SinPixeles) {
  // Entrada vacía
  SoA pixel_data;
  pixel_data.r = {};
  pixel_data.g = {};
  pixel_data.b = {};

  // Resultado esperado
  std::unordered_map<Pixel, int> const expected_frequencies = {};

  // Llamar a la función
  auto actual_frequencies = contarFrecuencias(pixel_data);

  // Verificar que no hay frecuencias
  EXPECT_EQ(actual_frequencies.size(), 0);
}

TEST(ContarFrecuenciasTest, PixelesDuplicados) {
  // Entrada con colores duplicados
  SoA pixel_data;
  pixel_data.r = {255, 255, 0, 0, 255};
  pixel_data.g = {0, 0, 255, 255, 0};
  pixel_data.b = {0, 0, 255, 255, 0};

  // Resultado esperado
  std::unordered_map<Pixel, int> const expected_frequencies = {
    {Pixel{255, 0, 0}, 3},   // Rojo: 3 apariciones
    {Pixel{0, 255, 255}, 2}  // Cian: 2 apariciones
  };

  // Llamar a la función
  auto actual_frequencies = contarFrecuencias(pixel_data);

  // Verificar las frecuencias
  EXPECT_EQ(actual_frequencies.size(), 2);
  EXPECT_EQ((actual_frequencies[Pixel{255, 0, 0}]), 3);
  EXPECT_EQ((actual_frequencies[Pixel{0, 255, 255}]), 2);
}

TEST(ContarFrecuenciasTest, PixelesDiversos) {
  // Entrada con múltiples colores únicos
  SoA pixel_data;
  pixel_data.r = {10, 20, 30, 40};
  pixel_data.g = {50, 60, 70, 80};
  pixel_data.b = {90, 100, 110, 120};

  // Resultado esperado
  std::unordered_map<Pixel, int> const expected_frequencies = {
    {Pixel{10, 50, 90}, 1},
    {Pixel{20, 60, 100}, 1},
    {Pixel{30, 70, 110}, 1},
    {Pixel{40, 80, 120}, 1}
  };

  // Llamar a la función
  auto actual_frequencies = contarFrecuencias(pixel_data);

  // Verificar que cada pixel único tiene una frecuencia de 1
  EXPECT_EQ(actual_frequencies.size(), 4);
  for (const auto& [pixel, freq] : expected_frequencies) {
    EXPECT_EQ(actual_frequencies[pixel], freq);
  }
}

/* menosFrecuentes unittests */
TEST(MenosFrecuentesTest, MenosFrecuentesExactos) {
  // Caso: número exacto de colores menos frecuentes
  std::unordered_map<Pixel, int> const colores = {
    {Pixel(255, 0, 0), 5},  // Rojo
    {Pixel(0, 255, 0), 3},  // Verde
    {Pixel(0, 0, 255), 1}   // Azul
  };

  BoundedPriorityQueue const resultado = menosFrecuentes(colores, 2);
  const auto& elements = resultado.get_all();

  // La cola debe contener solo los dos colores menos frecuentes
  EXPECT_EQ(elements.size(), 2);
  EXPECT_TRUE((elements[0] == Pixel(0, 0, 255) && elements[1] == Pixel(0, 255, 0)) ||
              (elements[0] == Pixel(0, 255, 0) && elements[1] == Pixel(0, 0, 255)));
}

TEST(MenosFrecuentesTest, FrecuenciaDuplicada) {
  // Caso: colores con la misma frecuencia
  std::unordered_map<Pixel, int> const colores = {
    {Pixel(255, 0, 0), 5},  // Rojo
    {Pixel(0, 255, 0), 5},  // Verde
    {Pixel(0, 0, 255), 10}  // Azul
  };

  BoundedPriorityQueue const resultado = menosFrecuentes(colores, 2);
  auto elements = resultado.get_all();

  // Los colores menos frecuentes deben estar en la cola
  EXPECT_EQ(elements.size(), 2);
  std::vector<Pixel> expected_pixels = {Pixel(255, 0, 0), Pixel(0, 255, 0)};
  EXPECT_TRUE(
      std::ranges::find(elements, expected_pixels[0]) != elements.end() ||
      std::ranges::find(elements, expected_pixels[1]) != elements.end()
  );
}

TEST(MenosFrecuentesTest, ColoresUnicos) {
  // Caso: cada color tiene una frecuencia distinta
  std::unordered_map<Pixel, int> const colores = {
    {Pixel(255, 0, 0), 3},  // Rojo
    {Pixel(0, 255, 0), 1},  // Verde
    {Pixel(0, 0, 255), 2},  // Azul
    {Pixel(255, 255, 0), 4} // Amarillo
  };

  BoundedPriorityQueue const resultado = menosFrecuentes(colores, 3);
  auto elements = resultado.get_all();

  // Los tres colores menos frecuentes deben estar en la cola
  EXPECT_EQ(elements.size(), 3);
  EXPECT_TRUE(std::ranges::find(elements, Pixel(0, 255, 0)) != elements.end()); // Verde
  EXPECT_TRUE(std::ranges::find(elements, Pixel(0, 0, 255)) != elements.end()); // Azul
  EXPECT_TRUE(std::ranges::find(elements, Pixel(255, 0, 0)) != elements.end()); // Rojo
}

/* calcularDistancia unittests*/
// Test para verificar la distancia entre píxeles idénticos
TEST(CalcularDistanciaTest, DistanciaCeroParaPixelesIguales) {
  // Crear dos píxeles idénticos
  Pixel const pixel1{255, 255, 255};  // Blanco
  Pixel const pixel2{255, 255, 255};  // Blanco

  // Calcular la distancia
  std::uint64_t const distancia = calcularDistancia(pixel1, pixel2);

  // Verificar que la distancia sea 0
  EXPECT_EQ(distancia, 0);
}

// Test para verificar la distancia entre dos píxeles diferentes
TEST(CalcularDistanciaTest, DistanciaEntrePixelesDiferentes) {
  // Crear dos píxeles diferentes
  Pixel const pixel1{255, 0, 0};    // Rojo
  Pixel const pixel2{0, 255, 0};    // Verde

  // Calcular la distancia
  std::uint64_t const distancia = calcularDistancia(pixel1, pixel2);

  // Verificar que la distancia no sea 0
  EXPECT_NE(distancia, 0);
}

// Test para verificar la fórmula con diferencias en los canales de color
TEST(CalcularDistanciaTest, DistanciaConDiferenciasEnCanales) {
  // Crear dos píxeles con diferencias en los canales
  Pixel const pixel1{100, 150, 200}; // Pixel 1
  Pixel const pixel2{200, 150, 100}; // Pixel 2

  // Calcular la distancia
  std::uint64_t const distancia = calcularDistancia(pixel1, pixel2);

  // Verificar que la distancia sea calculada correctamente (sin usar squared)
  std::uint64_t constexpr expected_distancia = (100 * 100) + (0 * 0) + (100 * 100); // (200-100)^2 + (150-150)^2 + (200-100)^2
  EXPECT_EQ(distancia, expected_distancia);
}

// Test para verificar la distancia cuando hay diferencias grandes en todos los canales
TEST(CalcularDistanciaTest, DistanciaConDiferenciasGrandes) {
  // Crear dos píxeles con diferencias grandes en todos los canales
  Pixel const pixel1{0, 0, 0};     // Negro
  Pixel const pixel2{255, 255, 255}; // Blanco

  // Calcular la distancia
  std::uint64_t const distancia = calcularDistancia(pixel1, pixel2);

  // La distancia esperada es la suma de las diferencias cuadradas en los tres canales
  std::uint64_t constexpr expected_distancia = (255 * 255) + (255 * 255) + (255 * 255); // (255-0)^2 + (255-0)^2 + (255-0)^2
  EXPECT_EQ(distancia, expected_distancia);
}

/* cutFreq unittests*/
TEST(CutFreqTest, ReemplazoMayorQueFrecuencias) {
  // Datos de prueba: solo hay un color rojo con alta frecuencia
  SoA pixel_data = {
    .r={255, 255, 255}, // R
    .g={0, 0, 0},       // G
    .b={0, 0, 0}        // B
  };

  // Llamada a la función con n_colors=1, por lo que el único color es el rojo
  cutfreq(pixel_data, 1);

  // Verificar que el color ha sido reemplazado por negro
  EXPECT_EQ(pixel_data.r[0], 0);
  EXPECT_EQ(pixel_data.g[0], 0);
  EXPECT_EQ(pixel_data.b[0], 0);
  EXPECT_EQ(pixel_data.r[1], 0);
  EXPECT_EQ(pixel_data.g[1], 0);
  EXPECT_EQ(pixel_data.b[1], 0);
  EXPECT_EQ(pixel_data.r[2], 0);
  EXPECT_EQ(pixel_data.g[2], 0);
  EXPECT_EQ(pixel_data.b[2], 0);
}

TEST(CutFreqTest, ReemplazoColoresMenosFrecuentes) {
  // Datos de prueba: hay 3 colores con diferentes frecuencias
  SoA pixel_data = {
    .r={255, 0, 0, 255, 255}, // R
    .g={0, 255, 0, 0, 0}, // G
    .b={0, 0, 255, 0, 0}, // B
  };

  // Llamada a la función con n_colors=2, se debería reemplazar el color azul
  cutfreq(pixel_data, 2);

  // Verificar que el color azul haya sido reemplazado por el verde o rojo
  EXPECT_TRUE((pixel_data.r[2] == 0 && pixel_data.g[2] == 255 && pixel_data.b[2] == 0) ||
              (pixel_data.r[2] == 255 && pixel_data.g[2] == 0 && pixel_data.b[2] == 0));

  // Verificar que los colores más frecuentes (rojo y verde) no hayan sido reemplazados
  EXPECT_TRUE((pixel_data.r[0] == 255 && pixel_data.g[0] == 0 && pixel_data.b[0] == 0) &&
              (pixel_data.r[1] == 255 && pixel_data.g[1] == 0 && pixel_data.b[1] == 0));
}

TEST(CutFreqTest, ColoresMasFrecuentesNoReemplazados) {
  // Datos de prueba: colores con diferentes frecuencias
  SoA pixel_data = {
    .r={255, 255, 255, 0, 0, 0},  // R
    .g={0, 0, 0, 255, 0, 255},  // G
    .b={0, 0, 0, 0, 255, 0},  // B
  };

  // Llamada a la función con n_colors=2, se debe mantener el color rojo (más frecuente)
  cutfreq(pixel_data, 2);

  // Verificar que los colores más frecuentes (rojo) no han sido reemplazados
  EXPECT_TRUE((pixel_data.r[0] == 255) && (pixel_data.g[0] == 0) && (pixel_data.b[0] == 0));
  EXPECT_TRUE((pixel_data.r[1] == 255) && (pixel_data.g[1] == 0) && (pixel_data.b[1] == 0));
  EXPECT_TRUE((pixel_data.r[2] == 255) && (pixel_data.g[2] == 0) && (pixel_data.b[2] == 0));

  // Verificar que el color azul ha sido reemplazado por el rojo
  EXPECT_TRUE((pixel_data.r[4] == 255) && (pixel_data.g[4] == 0) && (pixel_data.b[4] == 0));
}

TEST(CutFreqTest, ColoresNoReemplazadosSiNoSonMenosFrecuentes) {
  // Datos de prueba: colores con frecuencias variadas
  SoA pixel_data = {
    .r={255, 255, 0, 0},
    .g={0, 0, 255, 0},
    .b={0, 0, 0, 255},
  };

  // Llamada a la función con n_colors=1, se debe reemplazar el color azul
  cutfreq(pixel_data, 1);

  // Verificar que el color verde no ha sido reemplazado
  EXPECT_EQ(pixel_data.r[2], 0);
  EXPECT_EQ(pixel_data.g[2], 255);
  EXPECT_EQ(pixel_data.b[2], 0);

  EXPECT_EQ(pixel_data.r[3], 0);
  EXPECT_EQ(pixel_data.g[3], 255);
  EXPECT_EQ(pixel_data.b[3], 0);
}

// NOLINTEND(readability-magic-numbers)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)