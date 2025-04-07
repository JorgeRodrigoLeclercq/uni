#!/bin/bash

INPUT_DIR="inputs"
OUTPUT_DIR="outputs"
EXPOUT_DIR="expected_outputs/cutfreq"
IMTOOL_AOS="./release-build/imtool-aos/imtool-aos"

cd ..
if [ ! -d "release-build" ]; then
    mkdir release-build
    cmake . -DCMAKE_BUILD_TYPE=Release -B release-build
fi

# Test case para lake-large 100k
echo "Running cutfreq test for lake-large.ppm with cutfreq 100000"
if ! "$IMTOOL_AOS" "$INPUT_DIR/lake-large.ppm" "$OUTPUT_DIR/lake-large100k-AOS.ppm" cutfreq 100000; then
  echo "FAIL: Error generating lake-large100k-AOS.ppm."
else
  echo "PASS: lake-large100k-AOS.ppm generated successfully"
fi
# Test case para lake-large 162k
echo "Running cutfreq test for lake-large.ppm with cutfreq 162000"
if ! "$IMTOOL_AOS" "$INPUT_DIR/lake-large.ppm" "$OUTPUT_DIR/lake-large162k-AOS.ppm" cutfreq 162000; then
  echo "FAIL: Error generating lake-large162k-AOS.ppm."
else
  echo "PASS: lake-large162k-AOS.ppm generated successfully"
fi

echo "Imagenes generadas."
echo ""
echo "Comparando imagenes generadas con las esperadas..."

# Comparar las imágenes generadas
if ! diff "$OUTPUT_DIR/lake-large100k-AOS.ppm" "$EXPOUT_DIR/lake-large-100K.ppm"; then
  echo "FAIL: lake-large100k-AOS.ppm is different from lake-large-100K.ppm."
else
  echo "PASS: lake-large100k"
fi
# Comparar las imágenes generadas
if ! diff "$OUTPUT_DIR/lake-large162k-AOS.ppm" "$EXPOUT_DIR/lake-large162K.ppm"; then
  echo "FAIL: lake-large162k-AOS.ppm is different from lake-large-162K.ppm."
else
  echo "PASS: lake-large162k"
fi

echo "Tests finalizados."