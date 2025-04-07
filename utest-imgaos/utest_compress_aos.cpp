#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include "../imgaos/compress.hpp"
#include "../imgaos/info.hpp"

// Helper para leer contenido de un archivo
namespace {
  auto read_file(const char *path) -> std::string {
    std::ifstream infile(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>()};
  }
}

// Prueba para write_cppm_header
TEST(CompressTests, WriteCppmHeaderToFile) {
    const auto *temp_file = "test_header.cppm";
    std::ofstream outfile(temp_file, std::ios::binary);

    ImageHeader const header{
      "P6", {.width = 640, .height = 480},
       255
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
TEST(CompressTests, WriteColorTableToFile) {
    const auto *temp_file = "test_color_table.cppm";
    std::ofstream outfile(temp_file, std::ios::binary);

    std::vector<Pixel> const unique_colors = {
        Pixel(255, 0, 0),   // Rojo
        Pixel(0, 255, 0),   // Verde
        Pixel(0, 0, 255)    // Azul
    };

    constexpr size_t MAX_8_BIT = 255;
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
TEST(CompressTests, WriteCompressedPixelDataToFile) {
    const auto *temp_file = "test_pixel_data.cppm";
    std::ofstream outfile(temp_file, std::ios::binary);

    std::map<Pixel, uint32_t> const color_table = {
        {Pixel(255, 0, 0), 0},   // Rojo
        {Pixel(0, 255, 0), 1},   // Verde
        {Pixel(0, 0, 255), 2}    // Azul
    };
    std::vector<Pixel> const pixel_data = {
        Pixel(255, 0, 0),   // Rojo
        Pixel(0, 255, 0),   // Verde
        Pixel(0, 0, 255),   // Azul
        Pixel(255, 0, 0)    // Rojo
    };

    write_compressed_pixel_data(outfile, pixel_data, color_table, 1);
    outfile.close();

    // Leer contenido del archivo y verificar
    std::string const content = read_file(temp_file);
    std::string const expected = std::string("\x00\x01\x02\x00", 4); // Índices
    EXPECT_EQ(content, expected);

    const int success = std::remove(temp_file); // Eliminar archivo temporal
    EXPECT_EQ(success, 0);
}

// Prueba para compress
TEST(CompressTests, CompressToFile) {
    const auto *temp_file = "test_compress.cppm";
    std::ofstream outfile(temp_file, std::ios::binary);

    ImageHeader const header{"P6", {.width=2, .height=2}, 255};
    std::vector<Pixel> const pixel_data = {
        Pixel(255, 0, 0),   // Rojo
        Pixel(0, 255, 0),   // Verde
        Pixel(0, 0, 255),   // Azul
        Pixel(255, 0, 0)    // Rojo
    };

    compress(outfile, header, pixel_data);
    outfile.close();

    // Leer contenido del archivo y verificar
    std::string const content = read_file(temp_file);
    std::string const expected_header = "C6 2 2 255 3\n";
    std::string const expected_colors = std::string("\xFF\x00\x00", 3) +  // Rojo
                                   std::string("\x00\xFF\x00", 3) +  // Verde
                                   std::string("\x00\x00\xFF", 3);   // Azul
    auto const expected_data = std::string("\x00\x01\x02\x00", 4); // Índices
    EXPECT_EQ(content, expected_header + expected_colors + expected_data);

    const int success = std::remove(temp_file); // Eliminar archivo temporal
    EXPECT_EQ(success, 0);
}