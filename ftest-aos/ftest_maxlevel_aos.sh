#!/bin/bash

# Definir rutas
INPUT_DIR="../inputs"
OUTPUT_DIR="../outputs"
EXPECTED_DIR="../expected_outputs"
SOURCE_DIR="../imtool-aos"
IMGAOS_DIR="../imgaos"
IMTOOL_AOS="./imtool-aos"
COMMON_DIR="../common"

# Crear carpeta de salida si no existe
mkdir -p "$OUTPUT_DIR"

# Compilar el ejecutable
echo "Compiling imtool-aos..."
g++ -std=c++20 -o "$IMTOOL_AOS" -I"$COMMON_DIR" -I".." "$SOURCE_DIR/main.cpp" \
    "$IMGAOS_DIR/maxlevel.cpp" "$IMGAOS_DIR/info.cpp" \
    "$IMGAOS_DIR/compress.cpp" "$IMGAOS_DIR/cutfreq.cpp" "$IMGAOS_DIR/resize.cpp" \
    "$COMMON_DIR/binaryio.hpp" "$COMMON_DIR/pixel_structures.hpp" "$COMMON_DIR/cmp.cpp"

# Comprobar si la compilación fue exitosa
if [ ! -f "$IMTOOL_AOS" ]; then
  echo "Error: Compilation failed. Exiting."
  exit 1
fi

# Función para ejecutar pruebas
run_test() {
  local input_file=$1
  local maxlevel=$2
  local output_file=$3

  echo "Running maxlevel test for $input_file with maxlevel $maxlevel"

  # Leer el header de la imagen
  echo "Reading header for $input_file..."
  infile="$INPUT_DIR/$input_file"
  outfile="$OUTPUT_DIR/$output_file"

  # Ejecutar el programa con los parámetros necesarios
  $IMTOOL_AOS "$infile" "$outfile" maxlevel "$maxlevel"

  # Verificar si la ejecución fue exitosa
  if [ $? -ne 0 ]; then
    echo "FAIL: Error generating $output_file."
  else
    echo "PASS: $output_file generated successfully."
  fi
}

# Test cases para deer-small sin comparación de imágenes
run_test "deer-small.ppm" 100 "deer-small_100.ppm"
run_test "deer-small.ppm" 1000 "deer-small_1000.ppm"
run_test "deer-small.ppm" 65535 "deer-small_65535.ppm"

# Test case para lake-large
echo "Running maxlevel test for lake-large.ppm with maxlevel 65535"
$IMTOOL_AOS "$INPUT_DIR/lake-large.ppm" "$OUTPUT_DIR/lake-large_65535.ppm" maxlevel 65535

if [ $? -eq 0 ]; then
  echo "PASS: lake-large_65535.ppm generated successfully"
else
  echo "FAIL: Error generating lake-large_65535.ppm."
fi

echo "All tests completed."











