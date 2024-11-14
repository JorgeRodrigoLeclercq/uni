//
// Created by golden on 11/8/24.
//

#ifndef CUTFREQ_HPP
#define CUTFREQ_HPP
#include "common/priority_queue.hpp"
#include "common/pixel_structures.hpp"

#include <unordered_map>
#include <vector>


std::unordered_map<Pixel, int> contarFrecuencias(const SoA& pixel_data);
Bounded_priority_queue menosFrecuentes(const std::unordered_map<Pixel,int>& colores, int size);
std::uint64_t calcularDistancia(const Pixel &pixel1, const Pixel &pixel2);
void cutfreq(SoA &pixel_data, int n_colors);
#endif //CUTFREQ_HPP
