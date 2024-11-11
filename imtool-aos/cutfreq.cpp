//
// Created by golden on 11/8/24.
//

#include "cutfreq.hpp"
#include "common/priority_queue.hpp"
#include <unordered_map>
#include <cmath>


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

double calcularDistancia(const Pixel &pixel1, const Pixel &pixel2) {
  double suma = 0;
  suma += std::pow((pixel1.channels.red - pixel2.channels.red), 2);
  suma += std::pow((pixel1.channels.green - pixel2.channels.blue), 2);
  suma += std::pow((pixel1.channels.blue - pixel2.channels.blue), 2);

  return sqrt(suma);
}

void cutfreq(std::vector<Pixel> &pixel_data, int n_colors) {
  std::unordered_map<Pixel, int> const frecuencias = contarFrecuencias(pixel_data);
  Bounded_priority_queue<Pixel, int> const colores_menos_frecuentes = menosFrecuentes(frecuencias, n_colors);

  std::unordered_map<Pixel, std::pair<Pixel, double>> reemplazos;
  for (const auto &pixel : colores_menos_frecuentes.get_all()) {
    reemplazos[pixel] = std::make_pair(pixel, std::numeric_limits<double>::max());
  }
  for (const auto &color : frecuencias) {
    for (auto & [pixel_a_reemplazar, info_reemplazo] : reemplazos) {
      double const nueva_distancia = calcularDistancia(color.first, pixel_a_reemplazar);
      // Verificar que el color no sea uno a reemplazar
      if (!(reemplazos.contains(color.first)) && nueva_distancia < info_reemplazo.second) {
        info_reemplazo = std::make_pair(color.first, nueva_distancia);
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










