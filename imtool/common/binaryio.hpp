// binaryio.hpp
#ifndef BINARYIO_HPP
#define BINARYIO_HPP

#include <istream>
#include <ostream>

template <typename T>
T read_binary(std::istream &input) {
  T value;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  input.read(reinterpret_cast<char*>(&value), sizeof(value));
  return value;
}

template <typename T>
void write_binary(std::ostream &output, T const &value) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  output.write(reinterpret_cast<char const*>(&value), sizeof(value));
}

#endif // BINARYIO_HPP
