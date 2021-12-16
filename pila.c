#include "pila.h"

#include <stdlib.h>

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/
#define CAPACIDAD_INICIAL 10

pila_t *pila_crear(void){
  pila_t *pila = malloc(sizeof(pila_t));
  if(!pila){
    return NULL;
  }
  pila->cantidad = 0;
  pila->capacidad = CAPACIDAD_INICIAL;
  pila->datos = malloc(pila->capacidad * sizeof(void*));
  if(pila->datos == NULL){
    free(pila);
    return NULL;
  }
  return pila;

}

void pila_destruir(pila_t *pila){
  free(pila->datos);
  free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
  return(pila->cantidad == 0);
}

bool pila_redimensionar(pila_t *pila, size_t tam_nuevo) {
    void *datos_nuevo = realloc(pila->datos, tam_nuevo * sizeof(void*));
    if (tam_nuevo > 0 && datos_nuevo == NULL) {
        return false;
    }
    pila->datos = datos_nuevo;
    pila->capacidad = tam_nuevo;
    return true;
}

bool pila_apilar(pila_t *pila, void *valor){
  if(!pila){
    return false;
  }
  if(pila->cantidad == pila->capacidad){
    pila->capacidad *= 2;
    pila_redimensionar(pila,pila->capacidad);
    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    return true;
  }
  pila->datos[pila->cantidad] = valor;
  pila->cantidad++;
  return true;
}


void *pila_ver_tope(const pila_t *pila){
  if(pila_esta_vacia(pila)){
    return NULL;
  }
  return (pila->datos[pila->cantidad-1]);
}

void *pila_desapilar(pila_t *pila){
  if (pila_esta_vacia(pila)){
    return NULL;
  }
  void *valor = pila_ver_tope(pila);
  pila->cantidad--;
  if(pila->cantidad <= (pila->capacidad)/4 && (pila->capacidad)/2 >= CAPACIDAD_INICIAL){
    pila_redimensionar(pila,pila->capacidad/2);
    pila->capacidad /= 2;
  }
  return valor;
}
