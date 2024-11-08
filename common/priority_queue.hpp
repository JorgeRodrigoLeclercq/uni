//
// Created by golden on 11/8/24.
//

#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <vector>
#include <tuple>
#include <ostream>
#include <utility>
#include <iomanip>

template <typename Element_type, typename Priority_type> // Allows to use any type
class Bounded_priority_queue {
  // A `priority queue' is a queue-like structure where the elements are sorted
  // by a priority instead of the usual FIFO format. A `bounded priority queue'
  // is a priority queue, but when it is full, it discards the biggest element.
  public:
    constexpr Bounded_priority_queue (std::size_t max_capacity)
      : elements_(max_capacity, Element_type{}), priorities_(max_capacity, Priority_type{}) { }

    constexpr void enqueue (Element_type const & element,
                            Priority_type priority) {
      if (size_ == std::size(elements_)) {
          // The first element is the biggest one. If the element to be inserted has less priority than
          // the first one. Then remove the first one and append the new one.
          if (priority < priorities_[0]) {
            [[maybe_unused]] auto const dummy = dequeue_last();
            heap_append(element, priority);
          }
      } else {
        heap_append(element, priority);
      }
    }

    constexpr Element_type dequeue_last () {
      Element_type const element = std::move(elements_[0]);
      size_--;
      if (size_ != 0) {
        std::swap(elements_[0], elements_[size_]);
        std::swap(priorities_[0], priorities_[size_]);
        max_heapify(0);
      }
      return element;
    }

    constexpr std::vector<Element_type> const & get_all () const {
      return elements_;
    }

    friend std::ostream & operator<< (std::ostream & out,
                                      Bounded_priority_queue const & queue) {
      queue.print_helper(out, 0, 1);
      return out;
    }

  private:

    using index_type = std::ptrdiff_t;

    // We are going to store the binary tree in an array (AoCPv3 s5.2.3 p144)
    //
    //       +-------------------------------------+
    //       |                (0)                  |
    //       |            /         \              |
    //       |        (1)             (2)          |
    //       |      /    \          /     \        |
    //       |    (3)    (4)      (5)      (6)     |
    //       |    / \    /  \     /  \    /   \    |
    //       |  (7)(8) (9) (10) (11)(12) (13)(14)  |
    //       +-------------------------------------+
    //
    // That way the binary tree is `flat', all the nodes in the tree are
    // contiguous and there is no need for a `link' field in the node record.
    // This leverages the memory usage a lot. And reduces the number of caché
    // misses. Since all the child nodes are contiguous in memory. The original
    // implementation used 1-based arrays. In C++ we have to translate them to
    // 0-based ones.
    //
    //  - parent x      = floor ((x - 1) / 2)
    //  - left-child x  = x * 2 + 1
    //  - right-child x = x * 2 + 2

    constexpr static auto parent (index_type index) {
      return (index - 1) / 2;
    }

    constexpr static auto left_child (index_type index) {
      return index * 2 + 1;
    }

    constexpr static auto right_child (index_type index) {
      return index * 2 + 2;
    }

    // When we append to the heap, we just add the element to the end of the
    // vector and then propagate up. Swapping the child nodes with the parent
    // nodes when these don't follow the heap properties.

    constexpr void heap_append (Element_type const & element,
                                Priority_type priority) {
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

    void print_helper (std::ostream & out, index_type index, int depth) const {
      if (index >= size_) { return; }
      print_helper(out, left_child(index), depth + 1);
      out << std::setw(depth * 4) << "|- " << "(" << priorities_[index]
          << ", " << elements_[index] << ")\n";
      print_helper(out, right_child(index), depth + 1);
    }

    std::vector<Element_type>  elements_;
    std::vector<Priority_type> priorities_;
    index_type size_{};
};

#endif //PRIORITY_QUEUE_HPP
