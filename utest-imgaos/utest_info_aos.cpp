#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include <sstream>
#include "common/pixel_structures.hpp"
#include "common/header.hpp"
#include "imgaos/info.hpp"

// Helper function to create a temporary file with ppm data
void create_temp_ppm_file(const std::string& filename, const std::string& ppm_data) {
  std::ofstream file(filename, std::ios::binary);
  file << ppm_data;
  file.close();
}

// Test for get_pixels (8-bit)
TEST(GetPixelsTest, Reads8BitPixels) {
  // Mock input data for 8-bit image (magic number, dimensions, max color)
  std::string ppm_data = "P6\n2 2\n255\n\xff\x00\x00\x00\xff\x00\x00\x00\xff\xff\xff\x00\x00\xff";

  // Create temporary file for testing
  std::string filename = "test_image.ppm";
  create_temp_ppm_file(filename, ppm_data);

  // Open the file for reading
  std::ifstream input(filename, std::ios::binary);

  // Prepare pixel_data
  std::vector<Pixel> pixel_data(4);  // 2x2 image (4 pixels)
  unsigned long long pixel_count = 4;  // Number of pixels in the image
  bool is_16_bit = false;  // 8-bit image

  // Call function under test
  get_pixels(input, pixel_data, pixel_count, is_16_bit);

  // Check pixel values
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

  // Cleanup: remove the temporary file
  std::remove(filename.c_str());
}

// Test for get_pixels (16-bit)
TEST(GetPixelsTest, Reads16BitPixels) {
  // Mock input data for 16-bit image (magic number, dimensions, max color)
  std::string ppm_data = "P6\n2 2\n65535\n\xff\x00\x00\x00\xff\x00\x00\x00\xff\xff\xff\x00\x00\xff";

  // Create temporary file for testing
  std::string filename = "test_image.ppm";
  create_temp_ppm_file(filename, ppm_data);

  // Open the file for reading
  std::ifstream input(filename, std::ios::binary);

  // Prepare pixel_data
  std::vector<Pixel> pixel_data(4);  // 2x2 image (4 pixels)
  unsigned long long pixel_count = 4;  // Number of pixels in the image
  bool is_16_bit = true;  // 16-bit image

  // Call function under test
  get_pixels(input, pixel_data, pixel_count, is_16_bit);

  // Check pixel values for 16-bit data
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

  // Cleanup: remove the temporary file
  std::remove(filename.c_str());
}

// Helper function to read file into a string
std::string read_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// Test for write_info (8-bit)
TEST(WriteInfoTest, Writes8BitPixels) {
    // Mock ImageHeader
    ImageHeader header("P6", ImageDimensions{2, 2}, 255);  // 2x2 image, max color = 255
    // Mock pixel data (8-bit)
    std::vector<Pixel> pixel_data = {
        Pixel(255, 0, 0),   // Red
        Pixel(0, 255, 0),   // Green
        Pixel(0, 0, 255),   // Blue
        Pixel(0, 255, 255)  // Cyan
    };

    // Create temporary file for testing
    std::string filename = "test_output.ppm";
    std::ofstream outfile(filename, std::ios::binary);

    // Call function under test
    bool is_16_bit = false;  // 8-bit image
    write_info(outfile, header, pixel_data, is_16_bit);

    // Read the file content
    std::string file_content = read_file(filename);

    // Expected ppm content (Header + Pixel data)
    std::string expected_content = "P6\n2 2\n255\n\xff\x00\x00\x00\xff\x00\x00\x00\xff\xff\xff\x00\x00\xff";

    // Check if the file content matches the expected content
    EXPECT_EQ(file_content, expected_content);

    // Cleanup: remove the temporary file
    std::remove(filename.c_str());
}

// Test for write_info (16-bit)
TEST(WriteInfoTest, Writes16BitPixels) {
    // Mock ImageHeader
    ImageHeader header("P6", ImageDimensions{2, 2}, 65535);  // 2x2 image, max color = 65535

    // Mock pixel data (16-bit)
    std::vector<Pixel> pixel_data = {
        Pixel(65535, 0, 0),      // Red
        Pixel(0, 65535, 0),      // Green
        Pixel(0, 0, 65535),      // Blue
        Pixel(0, 65535, 65535)   // Cyan
    };

    // Create temporary file for testing
    std::string filename = "test_output.ppm";
    std::ofstream outfile(filename, std::ios::binary);

    // Call function under test
    bool is_16_bit = true;  // 16-bit image
    write_info(outfile, header, pixel_data, is_16_bit);

    // Read the file content
    std::string file_content = read_file(filename);

    // Expected ppm content (Header + Pixel data)
    std::string expected_content = "P6\n2 2\n65535\n\xff\x00\x00\x00\xff\x00\x00\x00\xff\xff\xff\x00\x00\xff";

    // Check if the file content matches the expected content
    EXPECT_EQ(file_content, expected_content);

    // Cleanup: remove the temporary file
    std::remove(filename.c_str());
}
