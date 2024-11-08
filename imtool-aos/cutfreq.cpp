//
// Created by golden on 11/8/24.
//

#include "cutfreq.hpp"
#include "common/priority_queue.hpp"
#include <unordered_map>


std::unordered_map<Pixel, int> contarFrecuencias(const std::vector<Pixel>& pixel_data) {
  std::unordered_map<Pixel, int> frecuencias;

  for (const auto &pixel : pixel_data) {
    frecuencias[pixel]++;
  }
  return frecuencias;
}

Bounded_priority_queue<Pixel, int> menosFrecuentes(const std::unordered_map<Pixel,int>& colores, int size) {
  Bounded_priority_queue<Pixel, int> colores_menos_frecuentes(size);

  for (const auto &color : colores) {
    colores_menos_frecuentes.enqueue(color.first, color.second);
  }
  return colores_menos_frecuentes;
}

void cutfreq(const std::vector<Pixel> &pixel_data, int n_colors) {
  std::unordered_map<Pixel, int> const frecuencias = contarFrecuencias(pixel_data);
  Bounded_priority_queue<Pixel, int> colores_menos_frecuentes = menosFrecuentes(frecuencias, n_colors);

  std::vector<Pixel> reemplazos = colores_menos_frecuentes.get_all();
  for (const auto &pixel : frecuencias) {

  }

}










