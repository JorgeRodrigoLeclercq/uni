#!/bin/bash

INPUT_DIR="inputs"
OUTPUT_DIR="outputs"
EXPOUT_DIR="expected_outputs"
IMTOOL_AOS="./release-build/imtool-aos/imtool-aos"

# Crear carpeta outputs si no existe

cd ..
if [ ! -d "$OUTPUT_DIR" ]; then
    mkdir "$OUTPUT_DIR"
    echo "Directorio '$OUTPUT_DIR' creado."
fi
if [ ! -d "release-build" ]; then
    mkdir release-build
    cmake . -DCMAKE_BUILD_TYPE=Release -B release-build
fi

# Test case para deer-small con maxlevel 100
echo "Running maxlevel test for deer-small.ppm with maxlevel 100"
if ! "$IMTOOL_AOS" "$INPUT_DIR/deer-small.ppm" "$OUTPUT_DIR/deer-small_100-AOS.ppm" maxlevel 100; then
  echo "FAIL: Error generating deer-small_100-AOS.ppm."
else
  echo "PASS: deer-small_100-AOS.ppm generated successfully"
fi

# Test case para deer-small con maxlevel 1000
echo "Running maxlevel test for deer-small.ppm with maxlevel 1000"
if ! "$IMTOOL_AOS" "$INPUT_DIR/deer-small.ppm" "$OUTPUT_DIR/deer-small_1000-AOS.ppm" maxlevel 1000; then
  echo "FAIL: Error generating deer-small_1000-AOS.ppm."
else
  echo "PASS: deer-small_1000-AOS.ppm generated successfully"
fi

# Test case para deer-small con maxlevel 65535
echo "Running maxlevel test for deer-small.ppm with maxlevel 65535"
if ! "$IMTOOL_AOS" "$INPUT_DIR/deer-small.ppm" "$OUTPUT_DIR/deer-small_65535-AOS.ppm" maxlevel 65535; then
  echo "FAIL: Error generating deer-small_65535-AOS.ppm."
else
  echo "PASS: deer-small_65535-AOS.ppm generated successfully"
fi

# Test case para lake-large con maxlevel 65535
echo "Running maxlevel test for lake-large.ppm with maxlevel 65535"
if ! "$IMTOOL_AOS" "$INPUT_DIR/lake-large.ppm" "$OUTPUT_DIR/lake-large_65535-AOS.ppm" maxlevel 65535; then
  echo "FAIL: Error generating lake-large_65535-AOS.ppm."
else
  echo "PASS: lake-large_65535-AOS.ppm generated successfully"
fi

echo "Imagenes generadas."
echo ""
echo "Comparando imágenes generadas con las esperadas..."

# Comparar las imágenes generadas
if ! diff "$OUTPUT_DIR/deer-small_100-AOS.ppm" "$EXPOUT_DIR/deer-small-100.ppm"; then
  echo "FAIL: deer-small_100-AOS.ppm is different from expected output."
else
  echo "PASS: deer-small_100"
fi
if ! diff "$OUTPUT_DIR/deer-small_1000-AOS.ppm" "$EXPOUT_DIR/deer-small-1000.ppm"; then
  echo "FAIL: deer-small_1000-AOS.ppm is different from expected output."
else
  echo "PASS: deer-small_1000"
fi
if ! diff "$OUTPUT_DIR/deer-small_65535-AOS.ppm" "$EXPOUT_DIR/deer-small-65535.ppm"; then
  echo "FAIL: deer-small_65535-AOS.ppm is different from expected output."
else
  echo "PASS: deer-small_65535"
fi

echo "Tests finalizados."

















