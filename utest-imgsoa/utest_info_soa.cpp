#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <sstream>
#include "../common/pixel_structures.hpp"
#include "common/header.hpp"
#include "imgsoa/info.hpp"

struct PPMFileData {
  std::string filename;
  std::vector<char> data;
};

namespace {
  void create_temp_ppm_file(PPMFileData const& ppm_file_data) {
    std::ofstream file(ppm_file_data.filename, std::ios::binary);
    file.write(ppm_file_data.data.data(), static_cast<std::streamsize>(ppm_file_data.data.size()));
    file.close();
  }

  auto read_file(const std::string& filename) -> std::string {
    std::ifstream file(filename, std::ios::binary);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
  }

  std::string capture_info_output(ImageHeader const & header, SoA const& pixel_data) {
    std::ostringstream const output;
    std::streambuf* original_cout_buffer = std::cout.rdbuf(output.rdbuf());

    info(header, pixel_data);

    std::cout.rdbuf(original_cout_buffer);
    return output.str();
  }
}

TEST(GetPixelsTest, Reads8BitPixels) {
  PPMFileData const ppm_file_data = {
    .filename="test_image.ppm",
    .data={'P', '6', '\n', '2', ' ', '2', '\n', '2', '5', '5', '\n',
     static_cast<char>(0xff), static_cast<char>(0x00), static_cast<char>(0x00),
     static_cast<char>(0x00), static_cast<char>(0xff), static_cast<char>(0x00),
     static_cast<char>(0x00), static_cast<char>(0x00), static_cast<char>(0xff),
     static_cast<char>(0xff), static_cast<char>(0xff), static_cast<char>(0x00),
     static_cast<char>(0x00), static_cast<char>(0xff)}
  };

  create_temp_ppm_file(ppm_file_data);

  std::ifstream input(ppm_file_data.filename, std::ios::binary);

  SoA pixel_data;
  constexpr unsigned long long pixel_count = 4;
  pixel_data.resize(pixel_count);

  constexpr bool is_16_bit = false;

  get_pixels(input, pixel_data, pixel_count, is_16_bit);

  EXPECT_EQ(pixel_data.r[0], 255);
  EXPECT_EQ(pixel_data.g[0], 0);
  EXPECT_EQ(pixel_data.b[0], 0);
  EXPECT_EQ(pixel_data.r[1], 0);
  EXPECT_EQ(pixel_data.g[1], 255);
  EXPECT_EQ(pixel_data.b[1], 0);
  EXPECT_EQ(pixel_data.r[2], 0);
  EXPECT_EQ(pixel_data.g[2], 0);
  EXPECT_EQ(pixel_data.b[2], 255);
  EXPECT_EQ(pixel_data.r[3], 0);
  EXPECT_EQ(pixel_data.g[3], 255);
  EXPECT_EQ(pixel_data.b[3], 255);

  input.close();
  int const success = std::remove(ppm_file_data.filename.c_str());
}

TEST(GetPixelsTest, Reads16BitPixels) {
  PPMFileData const ppm_file_data = {
    .filename="test_image.ppm",
    .data={'P', '6', '\n', '2', ' ', '2', '\n', '6', '5', '5', '3', '5', '\n',
           static_cast<char>(0xff), static_cast<char>(0x00), static_cast<char>(0x00),
           static_cast<char>(0x00), static_cast<char>(0xff), static_cast<char>(0x00),
           static_cast<char>(0x00), static_cast<char>(0x00), static_cast<char>(0xff),
           static_cast<char>(0xff), static_cast<char>(0xff), static_cast<char>(0x00),
           static_cast<char>(0x00), static_cast<char>(0xff)}
  };
  create_temp_ppm_file(ppm_file_data);

  std::ifstream input(ppm_file_data.filename, std::ios::binary);

  SoA pixel_data;
  constexpr unsigned long long pixel_count = 4;
  pixel_data.resize(pixel_count);

  constexpr bool is_16_bit = true;

  get_pixels(input, pixel_data, pixel_count, is_16_bit);

  EXPECT_EQ(pixel_data.r[0], 255);
  EXPECT_EQ(pixel_data.g[0], 0);
  EXPECT_EQ(pixel_data.b[0], 0);
  EXPECT_EQ(pixel_data.r[1], 0);
  EXPECT_EQ(pixel_data.g[1], 255);
  EXPECT_EQ(pixel_data.b[1], 0);
  EXPECT_EQ(pixel_data.r[2], 0);
  EXPECT_EQ(pixel_data.g[2], 0);
  EXPECT_EQ(pixel_data.b[2], 255);
  EXPECT_EQ(pixel_data.r[3], 0);
  EXPECT_EQ(pixel_data.g[3], 255);
  EXPECT_EQ(pixel_data.b[3], 255);

  input.close();
  int const success = std::remove(ppm_file_data.filename.c_str());
}

TEST(WriteInfoTest, Writes8BitPixels) {
    ImageHeader const header("P6", ImageDimensions{.width=2, .height=2}, 255);
    SoA pixel_data;
    constexpr int MAX_8_BIT = 255;
    pixel_data.resize(4);
    pixel_data.r = {MAX_8_BIT, 0, 0, 0};
    pixel_data.g = {0, MAX_8_BIT, 0, MAX_8_BIT};
    pixel_data.b = {0, 0, MAX_8_BIT, MAX_8_BIT};

    std::string const filename = "test_output.ppm";
    std::ofstream outfile(filename, std::ios::binary);

    constexpr bool is_16_bit = false;
    write_info(outfile, header, pixel_data, is_16_bit);

    std::string const file_content = read_file(filename);
    std::string const expected_content = {'P', '6', '\n', '2', ' ', '2', '\n', '2', '5', '5', '\n',
                                    static_cast<char>(0xff), static_cast<char>(0x00), static_cast<char>(0x00),
                                    static_cast<char>(0x00), static_cast<char>(0xff), static_cast<char>(0x00),
                                    static_cast<char>(0x00), static_cast<char>(0x00), static_cast<char>(0xff),
                                    static_cast<char>(0xff), static_cast<char>(0xff), static_cast<char>(0x00),
                                    static_cast<char>(0x00), static_cast<char>(0xff)};

    EXPECT_EQ(file_content, expected_content);

    int const success = std::remove(filename.c_str());
}

TEST(WriteInfoTest, Writes16BitPixels) {
    ImageHeader const header("P6", ImageDimensions{.width=2, .height=2}, 65535);
    SoA pixel_data;
    constexpr int MAX_16_BIT = 65535;
    pixel_data.resize(4);
    pixel_data.r = {MAX_16_BIT, 0, 0, 0};
    pixel_data.g = {0, MAX_16_BIT, 0, MAX_16_BIT};
    pixel_data.b = {0, 0, MAX_16_BIT, MAX_16_BIT};

    std::string const filename = "test_output.ppm";
    std::ofstream outfile(filename, std::ios::binary);

    constexpr bool is_16_bit = true;
    write_info(outfile, header, pixel_data, is_16_bit);

    std::string const file_content = read_file(filename);
    std::string const expected_content = {'P', '6', '\n', '2', ' ', '2', '\n', '6', '5', '5', '3', '5', '\n',
                                    static_cast<char>(0xff), static_cast<char>(0x00), static_cast<char>(0x00),
                                    static_cast<char>(0x00), static_cast<char>(0xff), static_cast<char>(0x00),
                                    static_cast<char>(0x00), static_cast<char>(0x00), static_cast<char>(0xff),
                                    static_cast<char>(0xff), static_cast<char>(0xff), static_cast<char>(0x00),
                                    static_cast<char>(0x00), static_cast<char>(0xff)};

    EXPECT_EQ(file_content, expected_content);

    int const success = std::remove(filename.c_str());
}

TEST(InfoFunctionTest, Prints8BitImageInfo) {
  ImageHeader const header{"P6", {.width=2, .height=2}, 255};
  SoA pixel_data;
  constexpr int MAX_8_BIT = 255;
  pixel_data.resize(4);
  pixel_data.r = {MAX_8_BIT, 0, 0, MAX_8_BIT};
  pixel_data.g = {0, MAX_8_BIT, 0, MAX_8_BIT};
  pixel_data.b = {0, 0, MAX_8_BIT, MAX_8_BIT};

  std::string const expected_output =
    "PPM Image Information:\n"
    "Magic Number: P6\n"
    "Dimensions: 2 x 2\n"
    "Max Color Value: 255\n\n"
    "Pixel Data:\n"
    "Pixel 1: R=255, G=0, B=0\n"
    "Pixel 2: R=0, G=255, B=0\n"
    "Pixel 3: R=0, G=0, B=255\n"
    "Pixel 4: R=255, G=255, B=255\n";

  std::string const actual_output = capture_info_output(header, pixel_data);
  EXPECT_EQ(actual_output, expected_output);
}

TEST(InfoFunctionTest, Prints16BitImageInfo) {
  ImageHeader const header{"P6", {.width=2, .height=2}, 65535};
  SoA pixel_data;
  constexpr int MAX_16_BIT = 65535;
  pixel_data.resize(4);
  pixel_data.r = {MAX_16_BIT, 0, 0, MAX_16_BIT};
  pixel_data.g = {0, MAX_16_BIT, 0, MAX_16_BIT};
  pixel_data.b = {0, 0, MAX_16_BIT, MAX_16_BIT};

  std::string const expected_output =
    "PPM Image Information:\n"
    "Magic Number: P6\n"
    "Dimensions: 2 x 2\n"
    "Max Color Value: 65535\n\n"
    "Pixel Data:\n"
    "Pixel 1: R=65535, G=0, B=0\n"
    "Pixel 2: R=0, G=65535, B=0\n"
    "Pixel 3: R=0, G=0, B=65535\n"
    "Pixel 4: R=65535, G=65535, B=65535\n";

  std::string const actual_output = capture_info_output(header, pixel_data);
  EXPECT_EQ(actual_output, expected_output);
}