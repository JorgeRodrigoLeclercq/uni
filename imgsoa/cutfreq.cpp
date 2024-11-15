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

BoundedPriorityQueue menosFrecuentes(const std::unordered_map<Pixel,int>& colores, int size) {
  BoundedPriorityQueue colores_menos_frecuentes(gsl::narrow<size_t>(size), Pixel{0,0,0});

  for (const auto &color : colores) {
    colores_menos_frecuentes.enqueue(color.first, color.second);
  }
  return colores_menos_frecuentes;
}

namespace {
  constexpr std::uint64_t squared (std::uint64_t numerito) {
    return numerito * numerito;
  }
}

std::uint64_t calcularDistancia(const Pixel &pixel1, const Pixel &pixel2) {
  std::uint64_t suma = 0;
  suma += squared(pixel1.channels.red - pixel2.channels.red);
  suma += squared(pixel1.channels.green - pixel2.channels.green);
  suma += squared(pixel1.channels.blue - pixel2.channels.blue);

  return suma;
}

void cutfreq(SoA &pixel_data, int n_colors) {
  std::unordered_map<Pixel, int> const frecuencias = contarFrecuencias(pixel_data);
  BoundedPriorityQueue const colores_menos_frecuentes = menosFrecuentes(frecuencias, n_colors);

  std::unordered_map<Pixel, std::pair<Pixel, std::uint64_t>> reemplazos;
  for (const auto &pixel : colores_menos_frecuentes.get_all()) {
    reemplazos[pixel] = std::make_pair(pixel, std::numeric_limits<std::uint64_t>::max());
  }
  std::vector<Pixel> colores_no_reemplazables;
  for (const auto &pixel : frecuencias) {
    if (!(reemplazos.contains(pixel.first))) {
      colores_no_reemplazables.push_back(pixel.first);
    }
  }
  for (auto & [pixel_a_reemplazar, info_reemplazo] : reemplazos) {
    for (const auto &color : colores_no_reemplazables) {
      std::uint64_t const nueva_distancia = calcularDistancia(color, pixel_a_reemplazar);
      // Verificar que el color no sea uno a reemplazar
      if (nueva_distancia < info_reemplazo.second) {
        info_reemplazo = std::make_pair(color, nueva_distancia);
      }
    }
  }

  for (std::size_t index = 0; index < pixel_data.r.size(); index++) {
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










