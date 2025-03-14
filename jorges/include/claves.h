#ifndef CLAVES_H
#define CLAVES_H

struct Coord {
    int x;
    int y;
};

/**
 * @brief Esta llamada permite inicializar el servicio de elementos key-value1-value2-value3.
 * Mediante este servicio se destruyen todas las tuplas que estuvieran almacenadas previamente.
 * 
 * @return int La función devuelve 0 en caso de éxito, -1 en caso de error en el servicio de tuplas
 *             y -2 en caso de error en la comunicación.
 */
int destroy(void);

/**
 * @brief Este servicio inserta el elemento <key, value1, value2, value3>.
 * El vector correspondiente al valor 2 vendrá dado por la dimensión del vector (N_value2) y el vector en sí (V_value2).
 * El servicio devuelve 0 si se insertó con éxito, -1 si se produce un error en el servicio (por ejemplo, si la clave ya existe
 * o si N_value2 está fuera del rango [1-32]) y -2 en caso de error de comunicaciones.
 * 
 * @param key       Clave.
 * @param value1    Cadena de caracteres (máximo 255 caracteres, sin contar el carácter nulo).
 * @param N_value2  Dimensión del vector V_value2 (entre 1 y 32).
 * @param V_value2  Vector de doubles (hasta 32 elementos).
 * @param value3    Estructura Coord.
 * @return int      0 si se insertó con éxito, -1 o -2 en caso de error.
 */
int set_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3);

/**
 * @brief Este servicio permite obtener los valores asociados a la clave key.
 * La cadena de caracteres se devuelve en value1, la dimensión del vector en N_value2, el vector en V_value2
 * y la estructura Coord en value3.
 * Tanto value1 como V_value2 deben tener espacio reservado para almacenar el máximo número de elementos
 * (256 para la cadena y 32 para el vector de doubles).
 * La función devuelve 0 en caso de éxito, -1 si no existe el elemento o hay error en el servicio
 * y -2 en caso de error en la comunicación.
 * 
 * @param key       Clave.
 * @param value1    Cadena de caracteres [256].
 * @param N_value2  Puntero a la dimensión del vector V_value2 [1-32].
 * @param V_value2  Vector de doubles [32].
 * @param value3    Puntero a la estructura Coord.
 * @return int      0 si se obtuvo con éxito, -1 o -2 en caso de error.
 */
int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3);

/**
 * @brief Este servicio permite modificar los valores asociados a la clave key.
 * La función devuelve 0 en caso de éxito, -1 si no existe el elemento o hay error en el servicio (o si N_value2
 * está fuera del rango) y -2 en caso de error en la comunicación.
 * 
 * @param key       Clave.
 * @param value1    Cadena de caracteres [256].
 * @param N_value2  Dimensión del vector V_value2 [1-32].
 * @param V_value2  Vector de doubles [32].
 * @param value3    Estructura Coord.
 * @return int      0 si se modificó con éxito, -1 o -2 en caso de error.
 */
int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3);

/**
 * @brief Este servicio permite borrar el elemento cuya clave es key.
 * La función devuelve 0 en caso de éxito, -1 si ocurre un error en el servicio (por ejemplo, la clave no existe)
 * y -2 en caso de error en la comunicación.
 * 
 * @param key       Clave.
 * @return int      0 si se eliminó con éxito, -1 o -2 en caso de error.
 */
int delete_key(int key);

/**
 * @brief Este servicio permite determinar si existe un elemento con clave key.
 * La función devuelve 1 en caso de que exista, 0 en caso de que no exista, -1 en caso de error en el servicio
 * y -2 en caso de error en la comunicación.
 * 
 * @param key       Clave.
 * @return int      1 si existe, 0 si no existe, -1 o -2 en caso de error.
 */
int exist(int key);

#endif
