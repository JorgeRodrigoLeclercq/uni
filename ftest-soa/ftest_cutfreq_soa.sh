#!/bin/bash

INPUT_DIR="inputs"
OUTPUT_DIR="outputs"
EXPOUT_DIR="expected_outputs/cutfreq"
IMTOOL_SOA="./release-build/imtool-soa/imtool-soa"

cd ..
if [ ! -d "release-build" ]; then
    mkdir release-build
    cmake . -DCMAKE_BUILD_TYPE=Release -B release-build
fi

# Test case para lake-large
echo "Running cutfreq test for lake-large.ppm with cutfreq 100000"
if ! "$IMTOOL_SOA" "$INPUT_DIR/lake-large.ppm" "$OUTPUT_DIR/lake-large100k-SOA.ppm" cutfreq 100000; then
  echo "FAIL: Error generating lake-large100k-SOA.ppm."
else
  echo "PASS: lake-large100k-SOA.ppm generated successfully"
fi
# Test case para lake-large
echo "Running cutfreq test for lake-large.ppm with cutfreq 162000"
if ! "$IMTOOL_SOA" "$INPUT_DIR/lake-large.ppm" "$OUTPUT_DIR/lake-large162k-SOA.ppm" cutfreq 162000; then
  echo "FAIL: Error generating lake-large162k-SOA.ppm."
else
  echo "PASS: lake-large162k-SOA.ppm generated successfully"
fi
echo "All tests completed."

echo "Imagenes generadas."
echo ""
echo "Comparando imagenes generadas con las esperadas..."

# Comparar las imágenes generadas
if ! diff "$OUTPUT_DIR/lake-large100k-SOA.ppm" "$EXPOUT_DIR/lake-large-100K.ppm"; then
  echo "FAIL: lake-large100k-SOA.ppm is different from lake-large-100K.ppm."
else
  echo "PASS: lake-large100k"
fi
# Comparar las imágenes generadas
if ! diff "$OUTPUT_DIR/lake-large162k-SOA.ppm" "$EXPOUT_DIR/lake-large162K.ppm"; then
  echo "FAIL: lake-large162k-SOA.ppm is different from lake-large-162K.ppm."
else
  echo "PASS: lake-large162k"
fi

echo "Tests finalizados."