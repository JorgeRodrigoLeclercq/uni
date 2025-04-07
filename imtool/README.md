Para compilar el código, se puede ejecutar el siguiente comando:
```bash
cmake . -DCMAKE_BUILD_TYPE=Release -B release-build
```
asegurandose de que existe la carpeta release-build.

# Importante

En caso de no compilar, se recomienda revisar el archivo
CMakelists.txt y verificar que la version de clang-tidy establecida
está instalada. En caso de ejecutarse en avignon, es importante
que sea clang-tidy-19.