//
// Created by golden on 11/8/24.
//

#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include "functions.hpp"
#include <vector>
#include <utility>

struct Pixel;

// Este algoritmo se ha extraido del libro The Art of Computer Programming vol.3, capitulo 5.2.3

class Bounded_priority_queue {
  // Implementado para limitar la estructura de cola de prioridad de std.
  public:
    constexpr Bounded_priority_queue (std::size_t max_capacity) : elements_(max_capacity, Pixel{}), priorities_(max_capacity, 0) { }

    constexpr void enqueue (Pixel element, int priority) {
      if (size_ == std::size(elements_)) {
          // Si el elemento tiene mayor prioridad que el último elemento de la cola de prioridad, lo eliminamos.
          if (priority < priorities_[0]) {
            dequeue_last();
            heap_append(element, priority);
          }
      } else {
        heap_append(element, priority); // añadir al final y propagar hacia arriba
      }
    }

    constexpr void dequeue_last () {
      size_--;
      if (size_ != 0) {
        std::swap(elements_[0], elements_[size_]);
        std::swap(priorities_[0], priorities_[size_]);
        max_heapify(0);
      }
    }

    [[nodiscard]] constexpr std::vector<Pixel> const & get_all () const {
      return elements_;
    }

  private:

    using index_type = std::size_t;

    constexpr static auto parent (index_type index) {
      return (index - 1) / 2;
    }

    constexpr static auto left_child (index_type index) {
      return (index * 2) + 1;
    }

    constexpr static auto right_child (index_type index) {
      return (index * 2) + 2;
    }

    // When we append to the heap, we just add the element to the end of the
    // vector and then propagate up. Swapping the child nodes with the parent
    // nodes when these don't follow the heap properties.

    constexpr void heap_append (Pixel const & element,
                                int priority) {
      elements_[size_] = element;
      priorities_[size_] = priority;
      size_++;
      propagate_up(size_ - 1);
    }

    constexpr void propagate_up (index_type index)
    // This function takes and index and propagates up the heap invariant:
    //
    //    «Every parent has more priority than both of its children»
    //
    // We start from index and check whether the parent has less priority than
    // its child, if so, swap them and go up.
    {
      while (index > 0 and priorities_[parent(index)] < priorities_[index]) {
        std::swap(elements_[index], elements_[parent(index)]);
        std::swap(priorities_[index], priorities_[parent(index)]);
        index = parent(index);
      }
    }

    constexpr void max_heapify (index_type index)
    // This procedure's purpose is mantaining the heap's properties of this
    // class's internal heap. It assumes that the subtree located at the given
    // nodes left and right children already satisfy the max-heap property. But
    // the tree at the index (current node) does not.
    {
      for (;;) {
        index_type const left = left_child (index);
        index_type const right = right_child (index);
        index_type largest = index;
        // Check which has the largest priority
        if (left < size_ and priorities_[largest] < priorities_[left]) {
          largest = left;
        }
        if (right < size_ and priorities_[largest] < priorities_[right]) {
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
    index_type size_{};
};

#endif //PRIORITY_QUEUE_HPP
