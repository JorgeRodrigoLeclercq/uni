//
// Created by golden on 11/8/24.
//

#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include "pixel_structures.hpp"
#include <vector>
#include <utility>
#include <iostream>


// Este algoritmo se ha extraido del libro The Art of Computer Programming vol.3, capitulo 5.2.3

class BoundedPriorityQueue {
  // Implementado para limitar la estructura de cola de prioridad de std.
  public:
  BoundedPriorityQueue (std::size_t max_capacity, [[maybe_unused]] Pixel initialiser) : elements_(max_capacity, Pixel{0,0,0}), priorities_(max_capacity, 0) { }

    void enqueue (Pixel element, int priority) {
      if (size_ == std::size(elements_)) {
          // Si el elemento tiene mayor prioridad que el último elemento de la cola de prioridad, lo eliminamos.
          // Esto incluye el caso de empate de colores, siguiendo la prioridad b->g->r.
          if (is_before(element, priority, elements_[0], priorities_[0])) {
            dequeue_last();
            heap_append(element, priority);
          }
      } else {
        heap_append(element, priority); // añadir al final y propagar hacia arriba
      }
    }

    void dequeue_last () {
      size_--;
      if (size_ != 0) {
        std::swap(elements_[0], elements_[size_]);
        std::swap(priorities_[0], priorities_[size_]);
        max_heapify();
      }
    }

    [[nodiscard]] std::vector<Pixel> const & get_all () const {
      return elements_;
    }

  private:
    static bool is_before (Pixel left_pixel, int left_priority, Pixel right_pixel, int right_priority) {
      return (left_priority < right_priority) or (left_priority == right_priority and left_pixel > right_pixel);
    }

    [[nodiscard]] bool is_before (std::size_t self, std::size_t other) const {
       return BoundedPriorityQueue::is_before(elements_[self], priorities_[self], elements_[other], priorities_[other]);
    }

    static auto parent (std::size_t index) {
      return (index - 1) / 2;
    }

     static auto left_child (std::size_t index) {
      return (index * 2) + 1;
    }

     static auto right_child (std::size_t index) {
      return (index * 2) + 2;
    }

     void heap_append (Pixel const & element,
                                int priority) {
      elements_[size_] = element;
      priorities_[size_] = priority;
      size_++;
      propagate_up(size_ - 1);
    }

     void propagate_up (std::size_t index)
    {
      while (index > 0 and is_before(parent(index), index)) {
        std::swap(elements_[index], elements_[parent(index)]);
        std::swap(priorities_[index], priorities_[parent(index)]);
        index = parent(index);
      }
    }

     void max_heapify ()
    {
      std::size_t index = 0;
      for (;;) {
        std::size_t const left = left_child(index);
        std::size_t const right = right_child(index);
        std::size_t largest = index;
        // Buscar mayor prioridad
        if (left < size_ and is_before(largest, left)) {
          largest = left;
        }
        if (right < size_ and is_before(largest, right)) {
          largest = right;
        }
        // If the element is already heapified, we are done.
        if (largest == index) { return; }
        // Swap them and heapify the children
        std::swap(elements_[index], elements_[largest]);
        std::swap(priorities_[index], priorities_[largest]);
        index = largest;
      }
    }

    std::vector<Pixel>  elements_;
    std::vector<int> priorities_;
    std::size_t size_{};
};

#endif //PRIORITY_QUEUE_HPP
