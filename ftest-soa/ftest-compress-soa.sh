#!/bin/bash

INPUT_DIR="inputs"
OUTPUT_DIR="outputs"
EXPOUT_DIR="expected_outputs/compress"
IMTOOL_SOA="./release-build/imtool-SOA/imtool-SOA"

cd ..
if [ ! -d "release-build" ]; then
    mkdir release-build
    cmake . -DCMAKE_BUILD_TYPE=Release -B release-build
fi

# Función para ejecutar pruebas
run_test() {
  local input_file=$1
  local compress=$2
  local output_file=$3

  echo "Running compress test for $input_file with compress $compress"

  # Leer el header de la imagen
  echo "Reading header for $input_file..."
  infile="$INPUT_DIR/$input_file"
  outfile="$OUTPUT_DIR/$output_file"

  # Ejecutar el programa con los parámetros necesarios
  if ! "$IMTOOL_SOA" "$infile" "$outfile" compress "$compress"; then
    echo "FAIL: Error generating $output_file."
  else
    echo "PASS: $output_file generated successfully."
  fi
}

# Test case para lake-small 
echo "Running compress test for lake-small.ppm with compress"
if ! "$IMTOOL_SOA" "$INPUT_DIR/lake-small.ppm" "$OUTPUT_DIR/lake-small-SOA.cppm" compress; then
  echo "FAIL: Error generating lake-small-SOA.cppm."
else
  echo "PASS: lake-small-SOA.cppm generated successfully"
fi
# Test case para deer-small 
echo "Running compress test for deer-small.ppm with compress"
if ! "$IMTOOL_SOA" "$INPUT_DIR/deer-small.ppm" "$OUTPUT_DIR/deer-small-SOA.cppm" compress; then
  echo "FAIL: Error generating deer-small-SOA.cppm."
else
  echo "PASS: deer-small-SOA.cppm generated successfully"
fi

echo "Imagenes generadas."
echo ""
echo "Comparando imagenes generadas con las esperadas..."

# Comparar las imágenes generadas
if ! diff "$OUTPUT_DIR/deer-small-SOA.cppm" "$EXPOUT_DIR/deer-small.cppm"; then
  echo "FAIL: deer-small-SOA.cppm is different from deer-small-cppm."
else
  echo "PASS: deer-small-SOA.cppm"
fi
# Comparar las imágenes generadas
if ! diff "$OUTPUT_DIR/lake-small-SOA.cppm" "$EXPOUT_DIR/lake-small.cppm"; then
  echo "FAIL: lake-small-SOA.cppm is different from lake-small.cppm."
else
  echo "PASS: lake-small-SOA.cppm"
fi

echo "Tests finalizados."