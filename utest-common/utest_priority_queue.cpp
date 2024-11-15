//
// Created by golden on 11/15/24.
//

#include "../common/priority_queue.hpp"
#include "gtest/gtest.h"
#include <vector>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(readability-magic-numbers)

// Test para verificar la inserción de elementos en la cola de prioridad
TEST(BoundedPriorityQueueTest, EnqueueTest) {
  Pixel const pixel1(255, 0, 0);
  Pixel const pixel2(0, 255, 0);
  Pixel const pixel3(0, 0, 255);
  BoundedPriorityQueue queue(3, pixel1);

  queue.enqueue(pixel1, 1);
  queue.enqueue(pixel2, 2);
  queue.enqueue(pixel3, 3);

  // Verificar el tamaño de la cola
  auto elements = queue.get_all();
  EXPECT_EQ(elements.size(), 3);

  // Verificar que el orden de los elementos es correcto (prioridad > color)
  EXPECT_EQ(elements[0], pixel3);  // Azul debería estar en la primera posición
  EXPECT_TRUE((elements[1] == pixel1 && elements[2] == pixel2) || (elements[1] == pixel2 && elements[2] == pixel1));
}

// Test para verificar que los píxeles se insertan según prioridad y color
TEST(BoundedPriorityQueueTest, ColorPriorityTest) {
  Pixel const red(255, 0, 0);
  Pixel const green(0, 255, 0);
  Pixel const blue(0, 0, 255);
  Pixel const yellow(0, 255, 255);  // Amarillo

  BoundedPriorityQueue queue(4, red);

  // Insertamos los píxeles en orden de menor a mayor prioridad
  queue.enqueue(red, 1);
  queue.enqueue(blue, 1);
  queue.enqueue(green, 1);
  queue.enqueue(yellow, 1);

  // Verificar el tamaño de la cola
  auto elements = queue.get_all();
  EXPECT_EQ(elements.size(), 4);

  // Verificar que los píxeles están ordenados correctamente por prioridad y color
  EXPECT_EQ(elements[0], yellow); // amarillo primero
  EXPECT_TRUE((elements[1] == blue && elements[2] == green) || (elements[1] == green && elements[2] == blue));
  EXPECT_EQ(elements[3], red); // rojo ultimo
}


// Test para verificar la eliminación del último elemento cuando la cola está llena, por prioridad
TEST(BoundedPriorityQueueTest, DequeueTest) {
  Pixel const pixel1(255, 0, 0);
  Pixel const pixel2(0, 255, 0);
  Pixel const pixel3(0, 0, 255);
  Pixel const yellow(0, 255, 255);  // Amarillo
  BoundedPriorityQueue queue(3, pixel1);

  queue.enqueue(pixel1, 2);  // Insertar un elemento con prioridad 1
  queue.enqueue(pixel2, 3);  // Insertar un elemento con prioridad 1
  queue.enqueue(pixel3, 4);  // Insertar un elemento con prioridad 2

  // Insertar un nuevo elemento con prioridad 1
  queue.enqueue(yellow, 1);

  auto elements = queue.get_all();
  EXPECT_EQ(elements.size(), 3);  // Debería seguir teniendo solo 3 elementos

  // El elemento de prioridad 4 (azul) deberia haber sido eliminado
  EXPECT_EQ(elements[0], pixel2);  // El verde debería estar en la primera posición por su prioridad
  EXPECT_TRUE((elements[1] == pixel1 && elements[2] == yellow) || (elements[1] == yellow && elements[2] == pixel1));
}

// Test para verificar la eliminación del último elemento cuando la cola está llena, por color
TEST(BoundedPriorityQueueTest, DequeueTestColor) {
  Pixel const pixel1(255, 0, 0);
  Pixel const pixel2(0, 255, 0);
  Pixel const pixel3(0, 0, 255);
  Pixel const yellow(0, 255, 255);  // Amarillo
  BoundedPriorityQueue queue(3, pixel1);

  queue.enqueue(pixel1, 1);
  queue.enqueue(yellow, 1);
  queue.enqueue(pixel3, 1);

  // Insertar el color verde
  queue.enqueue(pixel2, 1);

  auto elements = queue.get_all();
  EXPECT_EQ(elements.size(), 3);  // Debería seguir teniendo solo 3 elementos

  // El color mas grande deberia haber sido eliminado
  EXPECT_EQ(elements[0], pixel3);  // El azul debería estar en la primera posición por su prioridad
  EXPECT_TRUE((elements[1] == pixel1 && elements[2] == pixel2) || (elements[1] == pixel2 && elements[2] == pixel1));
}

// NOLINTEND(readability-magic-numbers)
// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)