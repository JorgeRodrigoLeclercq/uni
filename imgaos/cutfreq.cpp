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

BoundedPriorityQueue menosFrecuentes(const std::unordered_map<Pixel,int>& colores, int size) {
  BoundedPriorityQueue colores_menos_frecuentes(gsl::narrow<std::size_t>(size), Pixel{0,0,0});

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

void cutfreq(std::vector<Pixel> &pixel_data, int n_colors) {
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
      if (nueva_distancia < info_reemplazo.second) {
        info_reemplazo = std::make_pair(color, nueva_distancia);
      }
    }
  }
  for (auto &mypixel : pixel_data) {
    auto replace_pixel = reemplazos.find(mypixel); // comprobar si el pixel es uno a reemplazar
    if (replace_pixel != reemplazos.end()) {
      mypixel = replace_pixel->second.first;
    }
  }
}










