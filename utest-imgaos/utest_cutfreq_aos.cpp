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

/* contarFrecuencias unittests */
TEST(ContarFrecuenciasTest, PíxelesConFrecuenciasCorrectas) {
  std::vector<Pixel> const pixel_data = {
    Pixel(255, 0, 0),  // Rojo
    Pixel(0, 255, 0),  // Verde
    Pixel(255, 0, 0),  // Rojo
    Pixel(0, 0, 255)   // Azul
};

  std::unordered_map<Pixel, int> frecuencias = contarFrecuencias(pixel_data);

  // Verificar las frecuencias de los píxeles
  EXPECT_EQ(frecuencias[Pixel(255, 0, 0)], 2);  // Rojo debería aparecer 2 veces
  EXPECT_EQ(frecuencias[Pixel(0, 255, 0)], 1);  // Verde debería aparecer 1 vez
  EXPECT_EQ(frecuencias[Pixel(0, 0, 255)], 1);  // Azul debería aparecer 1 vez
}

TEST(ContarFrecuenciasTest, UnSoloPixel) {
  std::vector<Pixel> const pixel_data = { Pixel(255, 0, 0) };  // Un solo píxel rojo

  std::unordered_map<Pixel, int> frecuencias = contarFrecuencias(pixel_data);

  // Verificar que la frecuencia del píxel rojo es 1
  EXPECT_EQ(frecuencias[Pixel(255, 0, 0)], 1);
}

TEST(ContarFrecuenciasTest, VectorVacio) {
  std::vector<Pixel> constexpr pixel_data = {};  // Vector vacío

  std::unordered_map<Pixel, int> const frecuencias = contarFrecuencias(pixel_data);

  // Verificar que el mapa está vacío
  EXPECT_TRUE(frecuencias.empty());
}

TEST(ContarFrecuenciasTest, TodosLosPixelsIguales) {
  std::vector<Pixel> const pixel_data = { Pixel(255, 255, 255), Pixel(255, 255, 255), Pixel(255, 255, 255) };

  std::unordered_map<Pixel, int> frecuencias = contarFrecuencias(pixel_data);

  // Verificar que la frecuencia del píxel blanco es 3
  EXPECT_EQ(frecuencias[Pixel(255, 255, 255)], 3);
}

TEST(ContarFrecuenciasTest, VariosPixelsUnicos) {
  std::vector<Pixel> const pixel_data = {
    Pixel(255, 0, 0),  // Rojo
    Pixel(0, 255, 0),  // Verde
    Pixel(0, 0, 255),  // Azul
    Pixel(255, 255, 255) // Blanco
};

  std::unordered_map<Pixel, int> frecuencias = contarFrecuencias(pixel_data);

  // Verificar que cada píxel tiene una frecuencia de 1
  EXPECT_EQ(frecuencias[Pixel(255, 0, 0)], 1);
  EXPECT_EQ(frecuencias[Pixel(0, 255, 0)], 1);
  EXPECT_EQ(frecuencias[Pixel(0, 0, 255)], 1);
  EXPECT_EQ(frecuencias[Pixel(255, 255, 255)], 1);
}

TEST(ContarFrecuenciasTest, MezclaDePixelsRepetidosYUnicos) {
  std::vector<Pixel> const pixel_data = {
    Pixel(255, 0, 0),  // Rojo
    Pixel(255, 0, 0),  // Rojo
    Pixel(0, 255, 0),  // Verde
    Pixel(0, 255, 0),  // Verde
    Pixel(0, 0, 255),  // Azul
    Pixel(255, 255, 255) // Blanco
};

  std::unordered_map<Pixel, int> frecuencias = contarFrecuencias(pixel_data);

  // Verificar que las frecuencias son correctas
  EXPECT_EQ(frecuencias[Pixel(255, 0, 0)], 2);  // Rojo debería aparecer 2 veces
  EXPECT_EQ(frecuencias[Pixel(0, 255, 0)], 2);  // Verde debería aparecer 2 veces
  EXPECT_EQ(frecuencias[Pixel(0, 0, 255)], 1);  // Azul debería aparecer 1 vez
  EXPECT_EQ(frecuencias[Pixel(255, 255, 255)], 1);  // Blanco debería aparecer 1 vez
}

/* menosFrecuentes unittests */
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
  EXPECT_TRUE((elements[0] == Pixel{0, 0, 255} && elements[1] == Pixel{0, 255, 0})
          || (elements[0] == Pixel{0, 255, 0} && elements[1] == Pixel{0, 0, 255}));
  // Verificar que el color más frecuente (Rojo) no está en la cola
  EXPECT_FALSE((elements[1] ==  Pixel{255, 0, 0}) && (elements[2] == Pixel{255, 0, 0}));
}

TEST(MenosFrecuentesTest, ColaDeTamanoUno) {
  std::unordered_map<Pixel, int> const colores = {
    {Pixel(255, 0, 0), 5},  // Rojo, 5 veces
    {Pixel(0, 255, 0), 3},  // Verde, 3 veces
    {Pixel(0, 0, 255), 1}   // Azul, 1 vez
  };

  BoundedPriorityQueue const resultado = menosFrecuentes(colores, 1);
  const auto& elements = resultado.get_all();

  // Verificar que el tamaño de la cola es 1
  EXPECT_EQ(elements.size(), 1);

  // Verificar que el color menos frecuente (Azul) está en la cola
  EXPECT_EQ(elements[0], Pixel(0, 0, 255));
}

TEST(MenosFrecuentesTest, TodosLosColoresIguales) {
  std::unordered_map<Pixel, int> const colores = {
    {Pixel(255, 0, 0), 3},  // Rojo, 3 veces
    {Pixel(0, 255, 0), 3},  // Verde, 3 veces
    {Pixel(0, 0, 255), 3},  // Azul, 3 veces
};

  BoundedPriorityQueue const resultado = menosFrecuentes(colores, 2);
  const auto& elements = resultado.get_all();

  // Verificar que el tamaño de la cola es 2
  EXPECT_EQ(elements.size(), 2);

  // Verifica que el primer color es verde y el segundo es rojo
  EXPECT_TRUE(elements[0] == Pixel(0, 255, 0));
  EXPECT_TRUE(elements[1] == Pixel(255, 0, 0));
}

TEST(MenosFrecuentesTest, MasColoresQueLaCapacidad) {
  std::unordered_map<Pixel, int> const colores = {
    {Pixel(255, 0, 0), 5},   // Rojo, 5 veces
    {Pixel(0, 255, 0), 3},   // Verde, 3 veces
    {Pixel(0, 0, 255), 1},   // Azul, 1 vez
    {Pixel(255, 255, 0), 2}, // Amarillo, 2 veces
};

  BoundedPriorityQueue const resultado = menosFrecuentes(colores, 2);
  const auto& elements = resultado.get_all();

  // Verificar que el tamaño de la cola es 2
  EXPECT_EQ(elements.size(), 2);

  // Verificar que la cola contiene los 2 colores menos frecuentes
  EXPECT_TRUE((elements[0] == Pixel(0, 0, 255) && elements[1] == Pixel(255, 255, 0)) ||
              (elements[0] == Pixel(255, 255, 0) && elements[1] == Pixel(0, 0, 255)));
}

TEST(MenosFrecuentesTest, FrecuenciaDecreciente) {
  std::unordered_map<Pixel, int> const colores = {
    {Pixel(255, 0, 0), 1},   // Rojo, 1 vez
    {Pixel(0, 255, 0), 2},   // Verde, 2 veces
    {Pixel(0, 0, 255), 3},   // Azul, 3 veces
    {Pixel(255, 255, 0), 4}  // Amarillo, 4 veces
  };

  BoundedPriorityQueue const resultado = menosFrecuentes(colores, 3);
  const auto& elements = resultado.get_all();

  // Verificar que el tamaño de la cola es 3
  EXPECT_EQ(elements.size(), 3);

  // Verificar que el primer elemento es el color rojo, mas frecuente
  EXPECT_TRUE(elements[0] == Pixel(0, 0, 255));

  // Verificar que los elementos segundo y tercero son los colores verde y rojo, en cualquier orden
  EXPECT_TRUE((elements[1] == Pixel(0, 255, 0) && elements[2] == Pixel(255, 0, 0)) ||
              (elements[1] == Pixel(255, 0, 0) && elements[2] == Pixel(0, 255, 0)));
}

/* calcularDistancia unittests */
// Test para verificar la distancia entre píxeles idénticos
TEST(CalcularDistanciaTest, DistanciaCeroParaPíxelesIguales) {
  // Crear dos píxeles idénticos
  Pixel const pixel1{255, 255, 255};  // Blanco
  Pixel const pixel2{255, 255, 255};  // Blanco

  // Calcular la distancia
  std::uint64_t const distancia = calcularDistancia(pixel1, pixel2);

  // Verificar que la distancia sea 0
  EXPECT_EQ(distancia, 0);
}

// Test para verificar la distancia entre dos píxeles diferentes
TEST(CalcularDistanciaTest, DistanciaEntrePíxelesDiferentes) {
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


// NOLINTEND(readability-magic-numbers)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)