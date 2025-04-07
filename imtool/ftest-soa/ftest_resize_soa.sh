#!/bin/bash

INPUT_DIR="inputs"
OUTPUT_DIR="outputs"
EXPOUT_DIR="expected_outputs/resize"
IMTOOL_SOA="./release-build/imtool-soa/imtool-soa"

cd ..
if [ ! -d "release-build" ]; then
    mkdir release-build
    cmake . -DCMAKE_BUILD_TYPE=Release -B release-build
fi


# Test case para lake-large 100k
echo "Running resize for deer-small-100.ppm with dimension 1000 1000"
if ! "$IMTOOL_SOA" "$INPUT_DIR/deer-small-100.ppm" "$OUTPUT_DIR/deer-small-1000.ppm" resize 1000 1000; then
  echo "FAIL: Error generating deer-small-1000.ppm."
else
  echo "PASS: deer-small-1000.ppm generated successfully"
fi
# Test case para lake-large 162k
echo "Running resize for deer-large-1000.ppm with dimension 100 100"
if ! "$IMTOOL_SOA" "$INPUT_DIR/deer-large-1000.ppm" "$OUTPUT_DIR/deer-large-100.ppm" resize 100 100; then
  echo "FAIL: Error generating deer-large-100.ppm."
else
  echo "PASS: deer-large-100.ppm generated successfully"
fi

echo "Imagenes generadas."
echo ""
echo "Comparando imagenes generadas con las esperadas..."

# Comparar las imágenes generadas
if ! diff "$OUTPUT_DIR/deer-small-1000.ppm" "$EXPOUT_DIR/deer-small-1000K.ppm"; then
  echo "FAIL: deer-small-1000.ppm is different from deer-small-1000K.ppm."
else
  echo "PASS: deer-small-100k"
fi
# Comparar las imágenes generadas
if ! diff "$OUTPUT_DIR/deer-large-100.ppm" "$EXPOUT_DIR/deer-large-100K.ppm"; then
  echo "FAIL: deer-large-100.ppm is different from deer-large-100K.ppm."
else
  echo "PASS: deer-large-100k"
fi

echo "Tests finalizados."