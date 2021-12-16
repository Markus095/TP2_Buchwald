#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "pila.h"


typedef struct nodo{
  struct nodo* izq;
  struct nodo* der;
  char* clave;
  void* dato;
}nodo_abb_t;

struct abb{
  nodo_abb_t* raiz;
  size_t cantidad;
  abb_comparar_clave_t comparar_clave;
  abb_destruir_dato_t destruir_dato;
};


struct abb_iter{
  pila_t* pila;
};

nodo_abb_t* nodo_abb_crear(const char* clave, void* dato){
  nodo_abb_t* nodo=malloc(sizeof(nodo_abb_t));
  if (!nodo) {
    return NULL;
  }
  nodo->clave=strdup(clave);
  if (!nodo->clave) {
    free(nodo);
    return NULL;
  }
  nodo->dato=dato;
  nodo->izq=NULL;
  nodo->der=NULL;
  return nodo;
}

void* destruir_nodo_abb(nodo_abb_t* nodo){
  void* aux=nodo->dato;
  free(nodo->clave);
  free(nodo);
  return aux;

}
nodo_abb_t* _buscar_nodo_y_padre(nodo_abb_t** actual, nodo_abb_t* padre , const char* clave, abb_comparar_clave_t cmp){

  if (!(*actual)) { return padre; }

  if (!cmp((*actual)->clave, clave)) { return padre; }

  padre=*actual;

  if (cmp(clave, (*actual)->clave)>0) {
    *actual=(*actual)->der;

    return _buscar_nodo_y_padre(actual, padre, clave, cmp);
  }

  *actual=(*actual)->izq;

  return _buscar_nodo_y_padre(actual, padre, clave, cmp);
}

nodo_abb_t* buscar_nodo_y_padre(const abb_t* arbol, nodo_abb_t** nodo, const char* clave){

  *nodo=arbol->raiz;

  return _buscar_nodo_y_padre(nodo, NULL, clave, arbol->comparar_clave);
}


abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
  abb_t* arbol=malloc(sizeof(abb_t));
  if(!arbol){return NULL;}
  arbol->raiz=NULL;
  arbol->cantidad=0;
  arbol->comparar_clave=cmp;
  arbol->destruir_dato=destruir_dato;

  return arbol;
}

//Guarda el dato con la clave pasada en el arbol,
//indica si la operacion de guardado pudo o no
//llevarse a cabo
bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    nodo_abb_t* nodo;
    nodo_abb_t* padre=buscar_nodo_y_padre(arbol, &nodo, clave);
    if (!nodo) {
      nodo=nodo_abb_crear(clave, dato);
      if (!nodo) {
        return false;
      }
      arbol->cantidad++;
      if (!arbol->raiz) {
        arbol->raiz=nodo;
        return true;
      }
      if (arbol->comparar_clave(nodo->clave, padre->clave)>0) {
        padre->der=nodo;
      }else{
        padre->izq=nodo;
      }
      return true;
    }
    if (arbol->destruir_dato!=NULL) {
      arbol->destruir_dato(nodo->dato);
    }
    nodo->dato=dato;

    return true;
}

nodo_abb_t* obtener_reemplazo(nodo_abb_t* nodo){
  if(!nodo){return NULL;}
  if(!nodo->izq){ return nodo;}

  return obtener_reemplazo(nodo->izq);
}

void* _abb_borrar(abb_t* arbol, nodo_abb_t* nodo, const char* clave, abb_comparar_clave_t cmp){
  if(!nodo){return NULL;}
  nodo_abb_t* padre=buscar_nodo_y_padre(arbol,&nodo,clave);
  if(nodo==arbol->raiz){
    padre=nodo;
  }
  if(!padre){return NULL;}
  void* dato = nodo->dato;

  if(nodo->der && nodo->izq){
    nodo_abb_t* reemplazo = obtener_reemplazo(nodo->der);
    char* clave_reemplazo = strdup(reemplazo->clave);
    void* dato_reemplazo = _abb_borrar(arbol,reemplazo,reemplazo->clave,cmp);
    free(nodo->clave);
    nodo->clave = clave_reemplazo;
    nodo->dato = dato_reemplazo;

    return dato;
  }
  nodo_abb_t* nuevo_hijo = (nodo->der) ? nodo->der : nodo-> izq;
  if(padre->izq == nodo){
    padre->izq = nuevo_hijo;
  }else if(padre->der == nodo){
    padre->der = nuevo_hijo;
  }else{
    arbol->raiz = nuevo_hijo;
  }

  return destruir_nodo_abb(nodo);
}

void *abb_borrar(abb_t *arbol, const char *clave){
  nodo_abb_t* nodo;
  buscar_nodo_y_padre(arbol,&nodo,clave);
  if(!nodo){return NULL;}
  void* dato = _abb_borrar(arbol,nodo,clave,arbol->comparar_clave);
  arbol->cantidad--;

  return dato;
}


void *abb_obtener(const abb_t *arbol, const char *clave){
  if (!clave) {return NULL;}
  nodo_abb_t* nodo;
  buscar_nodo_y_padre(arbol, &nodo, clave);
  if (!nodo) {return NULL;}

  return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
  nodo_abb_t* nodo;
  buscar_nodo_y_padre(arbol,&nodo,clave);

  return(nodo !=NULL);
}

size_t abb_cantidad(const abb_t *arbol){
  return arbol->cantidad;
}

void _abb_destruir(nodo_abb_t* nodo, abb_destruir_dato_t destruir_dato){

  if (!nodo) {return;}
  _abb_destruir(nodo->izq, destruir_dato);
  _abb_destruir(nodo->der, destruir_dato);

  void* dato=destruir_nodo_abb(nodo);

  if (destruir_dato!=NULL) {
    destruir_dato(dato);
  }
}

void abb_destruir(abb_t *arbol){
  _abb_destruir(arbol->raiz,arbol->destruir_dato);
  free(arbol);
}

bool _abb_in_order(nodo_abb_t* actual, bool visitar(const char *, void *, void *), void *extra){

  if (!actual) {return true;}

  if (!_abb_in_order(actual->izq, visitar, extra)) {return false;}

  if (!visitar(actual->clave, actual->dato, extra)) {return false;}

  if (!_abb_in_order(actual->der, visitar, extra)) {return false;}

  return true;
}
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
  _abb_in_order(arbol->raiz, visitar,extra);
}


bool apilar_izquierdos(pila_t* pila, nodo_abb_t* nodo){
  if (!nodo) {return true;}
  return pila_apilar(pila, nodo)&&apilar_izquierdos(pila, nodo->izq);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){

  abb_iter_t* iter = malloc(sizeof(abb_iter_t));
  if(iter == NULL){return NULL;}

  iter->pila = pila_crear();

  if(!iter->pila){
    free(iter);
    return NULL;
  }

  if(arbol->raiz!=NULL){
    if (!apilar_izquierdos(iter->pila, arbol->raiz)) {
      abb_iter_in_destruir(iter);
      return NULL;
    }
  }

  return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){

  if (abb_iter_in_al_final(iter)) {return false;}

  nodo_abb_t* actual = pila_desapilar(iter->pila);
  if(actual->der!=NULL){
    if (!apilar_izquierdos(iter->pila, actual->der)) {
      return false;
    }
  }

  return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
  if (abb_iter_in_al_final(iter)) {
    return NULL;
  }

  return ((nodo_abb_t*)pila_ver_tope(iter->pila))->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){

  return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
  pila_destruir(iter->pila);
	free(iter);
}
