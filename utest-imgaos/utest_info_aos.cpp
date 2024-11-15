#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <sstream>
#include "../common/pixel_structures.hpp"
#include "common/header.hpp"
#include "imgaos/info.hpp"

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

  std::string capture_info_output(const ImageHeader& header, const std::vector<Pixel>& pixel_data) {
    std::ostringstream const output;
    std::streambuf* original_cout_buffer = std::cout.rdbuf(output.rdbuf());

    info(header, pixel_data);

    std::cout.rdbuf(original_cout_buffer);
    return output.str();
  }
}

// Test para get_pixels (8 bits)
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

  std::vector<Pixel> pixel_data(4);
  constexpr unsigned long long pixel_count = 4;
  constexpr bool is_16_bit = false;

  // Call function under test
  get_pixels(input, pixel_data, pixel_count, is_16_bit);

  EXPECT_EQ(pixel_data[0].channels.red, 255);
  EXPECT_EQ(pixel_data[0].channels.green, 0);
  EXPECT_EQ(pixel_data[0].channels.blue, 0);
  EXPECT_EQ(pixel_data[1].channels.red, 0);
  EXPECT_EQ(pixel_data[1].channels.green, 255);
  EXPECT_EQ(pixel_data[1].channels.blue, 0);
  EXPECT_EQ(pixel_data[2].channels.red, 0);
  EXPECT_EQ(pixel_data[2].channels.green, 0);
  EXPECT_EQ(pixel_data[2].channels.blue, 255);
  EXPECT_EQ(pixel_data[3].channels.red, 0);
  EXPECT_EQ(pixel_data[3].channels.green, 255);
  EXPECT_EQ(pixel_data[3].channels.blue, 255);

  input.close();
  int const success = std::remove(ppm_file_data.filename.c_str());
}

// Test para get_pixels (16 bits)
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

  std::vector<Pixel> pixel_data(4);
  constexpr unsigned long long pixel_count = 4;
  constexpr bool is_16_bit = true;

  get_pixels(input, pixel_data, pixel_count, is_16_bit);

  EXPECT_EQ(pixel_data[0].channels.red, 255);
  EXPECT_EQ(pixel_data[0].channels.green, 0);
  EXPECT_EQ(pixel_data[0].channels.blue, 0);
  EXPECT_EQ(pixel_data[1].channels.red, 0);
  EXPECT_EQ(pixel_data[1].channels.green, 255);
  EXPECT_EQ(pixel_data[1].channels.blue, 0);
  EXPECT_EQ(pixel_data[2].channels.red, 0);
  EXPECT_EQ(pixel_data[2].channels.green, 0);
  EXPECT_EQ(pixel_data[2].channels.blue, 255);
  EXPECT_EQ(pixel_data[3].channels.red, 0);
  EXPECT_EQ(pixel_data[3].channels.green, 255);
  EXPECT_EQ(pixel_data[3].channels.blue, 255);

  input.close();
  int const success = std::remove(ppm_file_data.filename.c_str());
}

// Test para write_info (8 bits)
TEST(WriteInfoTest, Writes8BitPixels) {
    ImageHeader const header("P6", ImageDimensions{.width=2, .height=2}, 255);
    std::vector<Pixel> const pixel_data = {
        Pixel(255, 0, 0),
        Pixel(0, 255, 0),
        Pixel(0, 0, 255),
        Pixel(0, 255, 255)
    };

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

// Test para write_info (16 bits)
TEST(WriteInfoTest, Writes16BitPixels) {
    ImageHeader const header("P6", ImageDimensions{.width=2, .height=2}, 65535);
    std::vector<Pixel> const pixel_data = {
        Pixel(65535, 0, 0),
        Pixel(0, 65535, 0),
        Pixel(0, 0, 65535),
        Pixel(0, 65535, 65535)
    };

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

// Test para info (8 bits)
TEST(InfoFunctionTest, Prints8BitImageInfo) {
  ImageHeader const header{"P6", {.width=2, .height=2}, 255};
  std::vector<Pixel> const pixel_data = {
    Pixel(255, 0, 0),
    Pixel(0, 255, 0),
    Pixel(0, 0, 255),
    Pixel(255, 255, 255)
  };

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

// Test para info (16 bits)
TEST(InfoFunctionTest, Prints16BitImageInfo) {
  ImageHeader const header{"P6", {.width=2, .height=2}, 65535};  // 2x2 image, max color = 65535 (16-bit)
  std::vector<Pixel> const pixel_data = {
    Pixel(65535, 0, 0),      // Red
    Pixel(0, 65535, 0),      // Green
    Pixel(0, 0, 65535),      // Blue
    Pixel(65535, 65535, 65535) // White
  };

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