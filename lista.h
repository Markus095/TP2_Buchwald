#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* **DEFINICIÓN DEL TIPO DE DATO** */
typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Devuelve verdadero si la lista no tiene elementos, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento al principio de la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al principio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento al final de la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del último elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el último elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

// Obtiene la cantidad de elementos contenidos en la lista.
// Pre: la lista fue creada.
// Post: se devolvió la cantidad de elementos en la lista.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));


/*********************** PRIMITIVAS DEL ITERADOR EXTERNO *********************/

//Pre: la lista fue creada.
//Pos: crea el iterador externo para la lista que recibe. Apunta al primer elemento. Devuelve NULL en caso de error
lista_iter_t *lista_iter_crear(lista_t *lista);

//Pre: el iterador externo fue creado.
//Pos: devuelve true si pudo apuntar al siguiente elemento (avanzar), false en caso contrario
bool lista_iter_avanzar(lista_iter_t *iter);

//Pre: el iterador externo fue creado.
//Pos: devuelve el dato del nodo actual al que apunta, o NULL si la lista está vacia
void *lista_iter_ver_actual(const lista_iter_t *iter);

//Pre: el iterador externo fue creado.
//Pos: devuelve true si pudo apuntar al ultimo elemento de la lista, false en caso contrario
bool lista_iter_al_final(const lista_iter_t *iter);

//Pre: el iterador externo fue creado.
//Pos: destruye el iterador externo.
void lista_iter_destruir(lista_iter_t *iter);

//Inserta un nuevo elemento en la posicion ANTERIOR a donde apunta el iterador.
//Pre: el iterador externo fue creado.
//Pos: devuelve true si pudo insertar el elemento o false en caso contrario
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

//Pre: el iterador externo fue creado.
//Pos: destruye el nodo actual y devuelve su dato o NULL en caso de error.
void *lista_iter_borrar(lista_iter_t *iter);




/**************************** PRIMITIVA DEL ITERADOR INTERNO ************************/
//pre: el iterador externo fue creado. Recibe la funcion visitar
//post:
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);


/**************************** PRUEBAS UNITARIAS ********************************/
void pruebas_lista_estudiante(void);



#endif //LISTA_H
