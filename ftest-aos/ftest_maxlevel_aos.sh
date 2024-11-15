#!/bin/bash

# Definir rutas
INPUT_DIR="../inputs"
OUTPUT_DIR="../outputs"
EXPECTED_DIR="../expected_outputs"
SOURCE_DIR="../imtool-aos"
IMGAOS_DIR="../imgaos"
IMTOOL_AOS="./imtool-aos"

# Crear carpeta de salida si no existe
mkdir -p "$OUTPUT_DIR"

# Compilar el ejecutable
echo "Compiling imtool-aos..."
g++ -o "$IMTOOL_AOS" -I".." "$SOURCE_DIR/main.cpp" \
    "$IMGAOS_DIR/maxlevel.cpp" "$IMGAOS_DIR/info.cpp" \
    "$IMGAOS_DIR/compress.cpp" "$IMGAOS_DIR/cutfreq.cpp" \
    "$IMGAOS_DIR/resize.cpp"

# Comprobar si la compilación fue exitosa
if [ ! -f "$IMTOOL_AOS" ]; then
  echo "Error: Compilation failed. Exiting."
  exit 1
fi

# Función para verificar si las imágenes generadas coinciden con las esperadas
compare_images() {
  local generated_file=$1
  local expected_file=$2

  # Comparar los archivos binarios
  if cmp -s "$generated_file" "$expected_file"; then
    echo "PASS: $generated_file matches $expected_file"
  else
    echo "FAIL: $generated_file does not match $expected_file"
  fi
}

# Función para ejecutar pruebas
run_test() {
  local input_file=$1
  local maxlevel=$2
  local output_file=$3
  local expected_file=$4

  echo "Running maxlevel test for $input_file with maxlevel $maxlevel"
  $IMTOOL_AOS "$INPUT_DIR/$input_file" "$OUTPUT_DIR/$output_file" maxlevel "$maxlevel"

  # Verificar si la ejecución fue exitosa
  if [ $? -ne 0 ]; then
    echo "FAIL: Error generating $output_file."
    return
  fi


  # Comparar si se proporciona un archivo esperado
  if [ -n "$expected_file" ]; then
    compare_images "$OUTPUT_DIR/$output_file" "$EXPECTED_DIR/$expected_file"
  fi
}
# Test cases para deer-small
run_test "deer-small.ppm" 100 "deer-small_100.ppm" "deer-small-100.ppm"
run_test "deer-small.ppm" 1000 "deer-small_1000.ppm" "deer-small-1000.ppm"
run_test "deer-small.ppm" 65535 "deer-small_65535.ppm" "deer-small-65535.ppm"
run_test "deer-small_65535.ppm" 255 "deer-small_255.ppm" "deer-small-255.ppm"

# Test case para lake-large (sin comparación)
echo "Running maxlevel test for lake-large.ppm with maxlevel 65535"
$IMTOOL_AOS "$INPUT_DIR/lake-large.ppm" "$OUTPUT_DIR/lake-large_65535.ppm" maxlevel 65535

if [ $? -eq 0 ]; then
  echo "PASS: lake-large_65535.ppm generated successfully (no comparison performed)."
else
  echo "FAIL: Error generating lake-large_65535.ppm."
fi

echo "All tests completed."