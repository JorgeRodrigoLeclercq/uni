#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include "../imgsoa/compress.hpp"
#include "../imgsoa/info.hpp"

// Helper para leer contenido de un archivo
namespace {
  auto read_file(const char *path) -> std::string {
    std::ifstream infile(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>()};
  }
}

// Prueba para write_cppm_header
TEST(CompressSoATests, WriteCppmHeaderToFile) {
    const auto *temp_file = "test_header.cppm";
    std::ofstream outfile(temp_file, std::ios::binary);

    ImageHeader const header{
        "P6", {.width = 640, .height = 480}, 255
    };
    constexpr size_t color_table_size = 128;

    write_cppm_header(outfile, header, color_table_size);
    outfile.close();

    // Leer contenido del archivo y verificar
    std::string const content = read_file(temp_file);
    std::string const expected = "C6 640 480 255 128\n";
    EXPECT_EQ(content, expected);

    const int success = std::remove(temp_file); // Eliminar archivo temporal
    EXPECT_EQ(success, 0);
}

// Prueba para write_color_table
TEST(CompressSoATests, WriteColorTableToFile) {
    const auto *temp_file = "test_color_table.cppm";
    std::ofstream outfile(temp_file, std::ios::binary);

    constexpr int MAX_8_BIT = 255;
    SoA unique_colors;
    unique_colors.resize(3);
    unique_colors.r = {MAX_8_BIT, 0, 0};   // Rojo
    unique_colors.g = {0, MAX_8_BIT, 0};   // Verde
    unique_colors.b = {0, 0, MAX_8_BIT};  // Azul

    write_color_table(outfile, unique_colors, MAX_8_BIT);
    outfile.close();

    // Leer contenido del archivo y verificar
    std::string const content = read_file(temp_file);
    std::string const expected = std::string("\xFF\x00\x00", 3) +  // Rojo
                                  std::string("\x00\xFF\x00", 3) +  // Verde
                                  std::string("\x00\x00\xFF", 3);   // Azul
    EXPECT_EQ(content, expected);

    const int success = std::remove(temp_file); // Eliminar archivo temporal
    EXPECT_EQ(success, 0);
}

// Prueba para write_compressed_pixel_data
TEST(CompressSoATests, WriteCompressedPixelDataToFile) {
    const auto *temp_file = "test_pixel_data.cppm";
    std::ofstream outfile(temp_file, std::ios::binary);

    constexpr int MAX_8_BIT = 255;
    SoA pixels;
    pixels.resize(4);
    pixels.r = {MAX_8_BIT, 0, 0, MAX_8_BIT};  // Rojo, Verde, Azul, Rojo
    pixels.g = {0, MAX_8_BIT, 0, 0};
    pixels.b = {0, 0, MAX_8_BIT, 0};

    std::map<std::size_t, uint32_t> const color_table = {
        {0, 0}, {1, 1}, {2, 2}, {3, 0}  // Índices en la tabla de colores
    };

    constexpr size_t index_size = 1;
    write_compressed_pixel_data(outfile, pixels, color_table, index_size);
    outfile.close();

    // Leer contenido del archivo y verificar
    std::string const content = read_file(temp_file);
    auto const expected = std::string("\x00\x01\x02\x00", 4); // Índices
    EXPECT_EQ(content, expected);

    const int success = std::remove(temp_file); // Eliminar archivo temporal
    EXPECT_EQ(success, 0);
}

// Prueba para compress
TEST(CompressSoATests, CompressToFile) {
    const auto *temp_file = "test_compress.cppm";
    std::ofstream outfile(temp_file, std::ios::binary);

    ImageHeader const header{"P6", {.width = 2, .height = 2}, 255};

    constexpr int MAX_8_BIT = 255;
    SoA pixel_data;
    pixel_data.resize(4);
    pixel_data.r = {MAX_8_BIT, 0, 0, MAX_8_BIT};   // Rojo, Verde, Azul, Rojo
    pixel_data.g = {0, MAX_8_BIT, 0, 0};
    pixel_data.b = {0, 0, MAX_8_BIT, 0};

    compress(outfile, header, pixel_data);
    outfile.close();

    // Leer contenido del archivo y verificar
    std::string const content = read_file(temp_file);
    std::string const expected_header = "C6 2 2 255 3\n";
    std::string const expected_colors = std::string("\xFF\x00\x00", 3) +  // Rojo
                                         std::string("\x00\xFF\x00", 3) +  // Verde
                                         std::string("\x00\x00\xFF", 3);   // Azul
    auto const expected_data = std::string("\x00\x01\x02\x00", 4);        // Índices
    EXPECT_EQ(content, expected_header + expected_colors + expected_data);

    const int success = std::remove(temp_file); // Eliminar archivo temporal
    EXPECT_EQ(success, 0);
}
