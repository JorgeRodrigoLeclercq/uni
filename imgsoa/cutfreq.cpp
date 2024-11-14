//
// Created by golden on 11/8/24.
//

#include "cutfreq.hpp"

#include "common/priority_queue.hpp"

#include <chrono>
#include <cmath>
#include <gsl/narrow>
#include <iostream>
#include <unordered_map>
#include <vector>

std::unordered_map<Pixel, int> contarFrecuencias(const SoA& pixel_data) {
  std::unordered_map<Pixel, int> frecuencias;
  size_t const maxsize = pixel_data.r.size();
  for (size_t index = 0; index < maxsize; index++) {
    frecuencias[Pixel{pixel_data.r[index],pixel_data.g[index], pixel_data.b[index]}]++;
  }
  return frecuencias;
}

Bounded_priority_queue menosFrecuentes(const std::unordered_map<Pixel,int>& colores, int size) {
  Bounded_priority_queue colores_menos_frecuentes(gsl::narrow<size_t>(size), Pixel{0,0,0});

  for (const auto &color : colores) {
    colores_menos_frecuentes.enqueue(color.first, color.second);
  }
  return colores_menos_frecuentes;
}

double calcularDistancia(const Pixel &pixel1, const Pixel &pixel2) {
  double suma = 0;
  suma += std::pow((pixel1.channels.red - pixel2.channels.red), 2);
  suma += std::pow((pixel1.channels.green - pixel2.channels.blue), 2);
  suma += std::pow((pixel1.channels.blue - pixel2.channels.blue), 2);

  return suma;
}

void cutfreq(SoA &pixel_data, int n_colors) {
  std::unordered_map<Pixel, int> const frecuencias = contarFrecuencias(pixel_data);
  for (auto const & [pixel, frecuencia] : frecuencias) {
    std::cout << "Frecuencia: " << frecuencia << "\n";
  }
  Bounded_priority_queue const colores_menos_frecuentes = menosFrecuentes(frecuencias, n_colors);

  std::unordered_map<Pixel, std::pair<Pixel, double>> reemplazos;
  for (const auto &pixel : colores_menos_frecuentes.get_all()) {
    reemplazos[pixel] = std::make_pair(pixel, std::numeric_limits<double>::max());
  }
  auto inicio = std::chrono::high_resolution_clock::now();
  std::cout << "Calculando los colores mas cercanos..." << "\n";
  for (const auto &color : frecuencias) {
    for (auto & [pixel_a_reemplazar, info_reemplazo] : reemplazos) {
      double const nueva_distancia = calcularDistancia(color.first, pixel_a_reemplazar);
      // Verificar que el color no sea uno a reemplazar
      if (!(reemplazos.contains(color.first)) && nueva_distancia < info_reemplazo.second) {
        info_reemplazo = std::make_pair(color.first, nueva_distancia);
      }
    }
  }
  auto fin = std::chrono::high_resolution_clock::now();
  auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
  std::cout << "Tiempo para calcular los mas cercanos: " << duracion << " microsegundos" << "\n";

  for (unsigned short const index : pixel_data.r) {
    Pixel new_pixel = Pixel{pixel_data.r[index],pixel_data.g[index], pixel_data.b[index]};
    auto replace_pixel = reemplazos.find(new_pixel); // comprobar si el pixel es uno a reemplazar
    if (replace_pixel != reemplazos.end()) {
      new_pixel = replace_pixel->second.first;
      pixel_data.r[index] = new_pixel.channels.red;
      pixel_data.g[index] = new_pixel.channels.green;
      pixel_data.b[index] = new_pixel.channels.blue;
    }
  }
}










