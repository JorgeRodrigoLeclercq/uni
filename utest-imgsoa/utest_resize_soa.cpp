//
// Created by parebu on 11/14/24.
//
#include <gtest/gtest.h>
#include <vector>
#include "../imgsoa/resize.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
// NOLINTBEGIN(readability-magic-numbers)

TEST(Resize_coordinates, same_Dimension) {
  ImageDimensions newDimensions;
  ImageDimensions oldDimensions;
  newDimensions.width = 10;
  newDimensions.height = 10;
  oldDimensions.width = 10;
  oldDimensions.height = 10;
  std::vector<float> coordinates_calculated= {0,0,0,0,0,0};
  const std::vector<float> coordinates_expected = {10,10,10,15,15,15};

  coordinates_calculated = coordinates_calculator( 10 , newDimensions, 15 , oldDimensions );

  EXPECT_EQ( coordinates_calculated, coordinates_expected );

}
//Correct coordinates when dimension is bigger
TEST(Resize_coordinates, diff_Dimension) {
  ImageDimensions newDimensions;
  ImageDimensions oldDimensions;
  newDimensions.width = 20;
  newDimensions.height = 20;
  oldDimensions.width = 10;
  oldDimensions.height = 10;
  std::vector<float> coordinates_calculated= {0,0,0,0,0,0};
  const std::vector<float> coordinates_expected = {5,5,5,7.5,7,8};

  coordinates_calculated = coordinates_calculator( 10 , newDimensions, 15 , oldDimensions );

  EXPECT_EQ( coordinates_calculated, coordinates_expected );

}
//Correct coordinates when dimension is smaller
TEST(Resize_coordinates, diff_lower_Dimension) {
  ImageDimensions newDimensions;
  ImageDimensions oldDimensions;
  newDimensions.width = 5;
  newDimensions.height = 5;
  oldDimensions.width = 10;
  oldDimensions.height = 10;
  std::vector<float> coordinates_calculated= {0,0,0,0,0,0};
  const std::vector<float> coordinates_expected = {20,20,20,30,30,30};

  coordinates_calculated = coordinates_calculator( 10 , newDimensions, 15 , oldDimensions );

  EXPECT_EQ( coordinates_calculated, coordinates_expected );

}

TEST(Resize_interpolation, normal_input) {
  ImageDimensions newDimensions;
  ImageDimensions oldDimensions;
  newDimensions.width = 20;
  newDimensions.height = 20;
  oldDimensions.width = 10;
  oldDimensions.height = 10;
  const uint16_t color1 = 50;
  const uint16_t color2 = 20;
  uint16_t output{};
  const uint16_t expected = 20;

  std::vector<float> coordinates_calculated= {0,0,0,0,0,0};

  coordinates_calculated = coordinates_calculator( 10 , newDimensions, 15 , oldDimensions );

  const float fraction = coordinates_calculated[0] - coordinates_calculated[1];
  output = interpolacion_correcta_colores(color1, fraction, color2);

  EXPECT_EQ(  expected, output );

}

TEST(Resize_interpolation, bigger_input) {
  ImageDimensions newDimensions;
  ImageDimensions oldDimensions;
  newDimensions.width = 20;
  newDimensions.height = 20;
  oldDimensions.width = 10;
  oldDimensions.height = 10;
  const uint16_t color1 = 500;
  const uint16_t color2 = 200;
  uint16_t output{};
  const uint16_t expected = 200;


  std::vector<float> coordinates_calculated= {0,0,0,0,0,0};

  coordinates_calculated = coordinates_calculator( 10 , newDimensions, 15 , oldDimensions );

  const float fraction = coordinates_calculated[0] - coordinates_calculated[1];
  output = interpolacion_correcta_colores(color1, fraction, color2);

  EXPECT_EQ(  expected, output );

}