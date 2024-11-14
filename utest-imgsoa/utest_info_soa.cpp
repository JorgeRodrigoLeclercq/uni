#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#include "PixelStructures.hpp"  // Assuming Pixel and ColorChannels are defined here

// Helper function to read file into a string
std::string read_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// Test for get_pixels (8-bit)
TEST(GetPixelsTest, Reads8BitPixels) {
    SoA pixel_data;
    unsigned long long pixel_count = 4;

    // Create mock input file
    std::string filename = "test_input.ppm";
    std::ofstream infile(filename, std::ios::binary);

    // Simulate PPM 8-bit image data (2x2, RGB pixels)
    infile << "P6\n2 2\n255\n";
    infile.write("\xff\x00\x00\x00\xff\x00\x00\x00\xff\xff\xff\x00\x00\xff", 12);  // 2x2 pixels

    infile.close();

    // Open the file for reading pixels
    std::ifstream file(filename, std::ios::binary);

    // Initialize SoA structure
    pixel_data.resize(pixel_count);

    // Read pixels into SoA
    bool is_16_bit = false;  // 8-bit image
    get_pixels(file, pixel_data, pixel_count, is_16_bit);

    // Check the pixel data
    EXPECT_EQ(pixel_data.r[0], 255);
    EXPECT_EQ(pixel_data.g[0], 0);
    EXPECT_EQ(pixel_data.b[0], 0);  // Red

    EXPECT_EQ(pixel_data.r[1], 0);
    EXPECT_EQ(pixel_data.g[1], 255);
    EXPECT_EQ(pixel_data.b[1], 0);  // Green

    EXPECT_EQ(pixel_data.r[2], 0);
    EXPECT_EQ(pixel_data.g[2], 0);
    EXPECT_EQ(pixel_data.b[2], 255);  // Blue

    EXPECT_EQ(pixel_data.r[3], 0);
    EXPECT_EQ(pixel_data.g[3], 255);
    EXPECT_EQ(pixel_data.b[3], 255);  // Cyan
}

// Test for get_pixels (16-bit)
TEST(GetPixelsTest, Reads16BitPixels) {
    SoA pixel_data;
    unsigned long long pixel_count = 4;

    // Create mock input file
    std::string filename = "test_input.ppm";
    std::ofstream infile(filename, std::ios::binary);

    // Simulate PPM 16-bit image data (2x2, RGB pixels)
    infile << "P6\n2 2\n65535\n";
    infile.write("\xff\xff\x00\x00\xff\xff\x00\x00\xff\xff\xff\x00\x00\xff", 12);  // 2x2 pixels

    infile.close();

    // Open the file for reading pixels
    std::ifstream file(filename, std::ios::binary);

    // Initialize SoA structure
    pixel_data.resize(pixel_count);

    // Read pixels into SoA
    bool is_16_bit = true;  // 16-bit image
    get_pixels(file, pixel_data, pixel_count, is_16_bit);

    // Check the pixel data
    EXPECT_EQ(pixel_data.r[0], 65535);
    EXPECT_EQ(pixel_data.g[0], 0);
    EXPECT_EQ(pixel_data.b[0], 0);  // Red

    EXPECT_EQ(pixel_data.r[1], 0);
    EXPECT_EQ(pixel_data.g[1], 65535);
    EXPECT_EQ(pixel_data.b[1], 0);  // Green

    EXPECT_EQ(pixel_data.r[2], 0);
    EXPECT_EQ(pixel_data.g[2], 0);
    EXPECT_EQ(pixel_data.b[2], 65535);  // Blue

    EXPECT_EQ(pixel_data.r[3], 0);
    EXPECT_EQ(pixel_data.g[3], 65535);
    EXPECT_EQ(pixel_data.b[3], 65535);  // Cyan
}

// Test for write_info (8-bit)
TEST(WriteInfoTest, Writes8BitPixels) {
    ImageHeader header("P6", ImageDimensions{2, 2}, 255);  // 2x2 image, max color = 255

    SoA pixel_data;
    pixel_data.resize(4);
    pixel_data.r = {255, 0, 0, 0};
    pixel_data.g = {0, 255, 0, 255};
    pixel_data.b = {0, 0, 255, 255};

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
    ImageHeader header("P6", ImageDimensions{2, 2}, 65535);  // 2x2 image, max color = 65535

    SoA pixel_data;
    pixel_data.resize(4);
    pixel_data.r = {65535, 0, 0, 0};
    pixel_data.g = {0, 65535, 0, 65535};
    pixel_data.b = {0, 0, 65535, 65535};

    // Create temporary file for testing
    std::string filename = "test_output.ppm";
    std::ofstream outfile(filename, std::ios::binary);

    // Call function under test
    bool is_16_bit = true;  // 16-bit image
    write_info(outfile, header, pixel_data, is_16_bit);

    // Read the file content
    std::string file_content = read_file(filename);

    // Expected ppm content (Header + Pixel data)
    std::string expected_content = "P6\n2 2\n65535\n\xff\xff\x00\x00\xff\xff\x00\x00\xff\xff\xff\x00\x00\xff";

    // Check if the file content matches the expected content
    EXPECT_EQ(file_content, expected_content);

    // Cleanup: remove the temporary file
    std::remove(filename.c_str());
}

// Test for info (8-bit)
TEST(InfoTest, Prints8BitPixels) {
    ImageHeader header("P6", ImageDimensions{2, 2}, 255);  // 2x2 image, max color = 255

    SoA pixel_data;
    pixel_data.resize(4);
    pixel_data.r = {255, 0, 0, 0};
    pixel_data.g = {0, 255, 0, 255};
    pixel_data.b = {0, 0, 255, 255};

    testing::internal::CaptureStdout();
    info(header, pixel_data);
    std::string output = testing::internal::GetCapturedStdout();

    // Verify the output
    std::string expected_output = "PPM Image Information:\nMagic Number: P6\nDimensions: 2 x 2\nMax Color Value: 255\n\nPixel Data:\nPixel 1: R=255, G=0, B=0\nPixel 2: R=0, G=255, B=0\nPixel 3: R=0, G=0, B=255\nPixel 4: R=0, G=255, B=255\n";

    EXPECT_EQ(output, expected_output);
}

// Test for info (16-bit)
TEST(InfoTest, Prints16BitPixels) {
    ImageHeader header("P6", ImageDimensions{2, 2}, 65535);  // 2x2 image, max color = 65535

    SoA pixel_data;
    pixel_data.resize(4);
    pixel_data.r = {65535, 0, 0, 0};
    pixel_data.g = {0, 65535, 0, 65535};
    pixel_data.b = {0, 0, 65535, 65535};

    testing::internal::CaptureStdout();
    info(header, pixel_data);
    std::string output = testing::internal::GetCapturedStdout();

    // Verify the output
      std::string expected_output = "PPM Image Information:\nMagic Number: P6\nDimensions: 2 x 2\nMax Color Value: 65535\n\nPixel Data:\nPixel 1: R=65535, G=0, B=0\nPixel 2: R=0, G=65535, B=0\nPixel 3: R=0, G=0, B=65535\nPixel 4: R=0, G=65535, B=65535\n";

    EXPECT_EQ(output, expected_output);
}