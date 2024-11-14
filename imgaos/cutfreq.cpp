//
// Created by golden on 11/8/24.
//

#include "cutfreq.hpp"

#include "common/priority_queue.hpp"

#include <chrono>
#include <cmath>
#include <gsl/gsl>
#include <iostream>
#include <unordered_map>

std::unordered_map<Pixel, int> contarFrecuencias(const std::vector<Pixel>& pixel_data) {
  std::unordered_map<Pixel, int> frecuencias;

  for (const auto &pixel : pixel_data) {
    frecuencias[pixel]++;
  }
  return frecuencias;
}

Bounded_priority_queue menosFrecuentes(const std::unordered_map<Pixel,int>& colores, int size) {
  Bounded_priority_queue colores_menos_frecuentes(gsl::narrow<std::size_t>(size), Pixel{0,0,0});

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

void cutfreq(std::vector<Pixel> &pixel_data, int n_colors) {
  std::unordered_map<Pixel, int> const frecuencias = contarFrecuencias(pixel_data);
  Bounded_priority_queue const colores_menos_frecuentes = menosFrecuentes(frecuencias, n_colors);

  std::unordered_map<Pixel, std::pair<Pixel, double>> reemplazos;
  for (const auto &pixel : colores_menos_frecuentes.get_all()) {
    reemplazos[pixel] = std::make_pair(pixel, std::numeric_limits<double>::max());
  }
  std::vector<Pixel> colores_no_reemplazables;
  for (const auto &pixel : frecuencias) {
    if (!(reemplazos.contains(pixel.first))) {
      colores_no_reemplazables.push_back(pixel.first);
    }
  }
  auto inicio = std::chrono::high_resolution_clock::now();
  std::cout << "Calculando los colores mas cercanos..." << "\n";
  for (auto & [pixel_a_reemplazar, info_reemplazo] : reemplazos) {
    for (const auto &color : colores_no_reemplazables) {
      double const nueva_distancia = calcularDistancia(color, pixel_a_reemplazar);
      if (nueva_distancia < info_reemplazo.second) {
        info_reemplazo = std::make_pair(color, nueva_distancia);
      }
    }
  }
  auto fin = std::chrono::high_resolution_clock::now();
  auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(fin - inicio).count();
  std::cout << "Tiempo para calcular los mas cercanos: " << duracion << " microsegundos" << "\n";
  for (auto &mypixel : pixel_data) {
    auto replace_pixel = reemplazos.find(mypixel); // comprobar si el pixel es uno a reemplazar
    if (replace_pixel != reemplazos.end()) {
      mypixel = replace_pixel->second.first;
    }
  }
}










