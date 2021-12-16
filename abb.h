#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

//Crea un abb
//Pre: recibe una funcion para comparar las claves y otra para destruir los datos almacenados
//Pos: devuelve un abb vacio o NULL en caso de error.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

//Guarda el dato y su clave en el arbol
//Pre: el arbol fue creado, la clave es valida
//Pos: devuelve true si se guardaron correctamente o false en caso contrario.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

//Borra del abb el nodo correspondiente a la clave recibida
//Pre: el arbol fue creado, la clave es valida.
//Pos: devuelve el dato de ese nodo o NULL en caso de no encontrarlo.
void *abb_borrar(abb_t *arbol, const char *clave);

//Obtiene el dato del nodo que corresponde a la clave recibida
//Pre: el arbol fue creado, la clave es valida.
//Pos: devuelve el dato previamente mencionado o NULL en caso de no encontrar el nodo.
void *abb_obtener(const abb_t *arbol, const char *clave);

//Indica si la clave recibida se encuentra o no en el arbol
//Pre: el arbol fue creado, la clave es valida.
//Pos: devuelve true si la clave se encuentra en el arbol o false en caso contrario.
bool abb_pertenece(const abb_t *arbol, const char *clave);

//Pre: el arbol fue creado.
//Pos: devuelve la cantidad de elementos almacenados en el arbol.
size_t abb_cantidad(const abb_t *arbol);

//Destruye el arbol y todos sus datos con la funcion de destruccion.
//Pre: el arbol fue creado.
void abb_destruir(abb_t *arbol);


/* PRIMITIVA DEL ITERADOR INTERNO */

//Pre: el arbol fue creado. La funcion visitar puede ser true, false, o NULL.
//Pos: recorre el abb completo o hasta que se cumpla la condicion de corte (visitar == false)
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);


/* PRIMITIVAS Y ESTRUCTURA DEL ITERADOR EXTERNO */
typedef struct abb_iter abb_iter_t;

//Crea el iterador externo del abb.
//Pre: el arbol fue creado
//Pos: devuelve el iterador o NULL en caso de error.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

//Avanza el iterador una posicion
//Pre: el iterador fue creado.
//Pos: devuelve true si pudo avanzar o false en caso contrario.
bool abb_iter_in_avanzar(abb_iter_t *iter);

//Muestra la clave del elemento al que apunta actualmente el iterador
//Pre: el iterador fue creado.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

//Pre: el iterador fue creado.
//Pos: devuelve true si el iterador se encuentra en el ultimo elemento o false en caso contrario
bool abb_iter_in_al_final(const abb_iter_t *iter);

//Destruye el iterador
//Pre: el iterador fue creado
void abb_iter_in_destruir(abb_iter_t* iter);

#endif
