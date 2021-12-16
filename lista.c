#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>


typedef struct nodo nodo_t;

struct nodo {
    void* dato;
    nodo_t* prox;  // Puntero al siguiente nodo.
};

typedef struct lista {
    nodo_t* primero;   // Puntero al primer nodo de la lista.
    nodo_t* ultimo;  // Puntero al último nodo de la lista.
    size_t largo;
}lista_t;

typedef struct lista_iter {
	lista_t* lista;
	struct nodo* act;
	struct nodo* ant;
} lista_iter_t;


nodo_t *nodo_crear(void* dato){
  nodo_t *nodo = malloc(sizeof(nodo_t));
  if(!nodo){
    return NULL;
  }
  nodo->dato = dato;
  nodo->prox = NULL;
  return nodo;
}

void destruir_nodo(nodo_t* nodo){
	free(nodo);
}

lista_t *lista_crear(void){
  lista_t *lista = malloc(sizeof(lista_t));
  if(!lista){
    return NULL;
  }
  lista->ultimo = NULL;
  lista->primero = NULL;
  lista->largo = 0;
  return lista;
}

bool lista_esta_vacia(const lista_t *lista){
  return lista->largo == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
  nodo_t *nodo = nodo_crear(dato);
  if(!nodo){
    return false;
  }
  if(lista_esta_vacia(lista)){
    lista->ultimo = nodo;
  }
  if(!lista_esta_vacia(lista)){
    nodo->prox = lista->primero;
  }
  lista->primero = nodo;
  lista->largo += 1;
  return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
  nodo_t *nodo = nodo_crear(dato);
  if(!nodo){
    return false;
  }
  if(lista_esta_vacia(lista)){
    lista->primero = nodo;
  }
  if(!lista_esta_vacia(lista)){
    lista->ultimo->prox = nodo;
  }
  lista->ultimo = nodo;
  lista->largo += 1;
  return true;
}

void *lista_borrar_primero(lista_t *lista){
  if(lista_esta_vacia(lista)){return NULL;}
  
  nodo_t* aux = lista->primero;
  void* aux_dato = lista->primero->dato;
  lista->primero = lista->primero->prox;
  lista->largo -= 1;
  free(aux);
  return aux_dato;
}



void *lista_ver_primero(const lista_t *lista){
  return lista_esta_vacia(lista) ?  NULL : lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista){
  return lista_esta_vacia(lista) ?  NULL : lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista){
  return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){
  void* aux;
  while(!lista_esta_vacia(lista)){
    aux = lista_borrar_primero(lista);
    if(destruir_dato != NULL){
      destruir_dato(aux);
    }
  }
  free(lista);

}


lista_iter_t *lista_iter_crear(lista_t *lista){
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if(!iter){
		return NULL;
	}
	iter->lista = lista;
	iter->act = lista->primero;
	iter->ant = NULL;
	return iter;
}

bool lista_iter_al_final(const lista_iter_t *iter){
  if(lista_esta_vacia(iter->lista)){return true;}
	return (iter->ant == iter->lista->ultimo);
}

bool lista_iter_avanzar(lista_iter_t *iter){
	if(lista_iter_al_final(iter)){
		return false;
	}
	iter->ant = iter->act;
	iter->act = iter->act->prox;
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
	return iter->act == NULL ? NULL : iter->act->dato;
}


void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	nodo_t *nodo_nuevo = nodo_crear(dato);
	if(!nodo_nuevo){
		return false;
	}
	if(iter->ant != NULL){
		iter->ant->prox = nodo_nuevo;
	}
  if(iter->act == iter->lista->primero){
    iter->lista->primero = nodo_nuevo;
  }
  if(lista_iter_al_final(iter)){
    iter->lista->ultimo = nodo_nuevo;
  }
	nodo_nuevo->prox = iter->act;
	iter->act = nodo_nuevo;
  iter->lista->largo +=1;
	return true;
}

void *lista_iter_borrar(lista_iter_t *iter){
  if(lista_esta_vacia(iter->lista)||lista_iter_al_final(iter)){return NULL;}
  if(iter->act == iter->lista->primero){
    iter->lista->primero = iter->act->prox;
  }
  if(iter->act == iter->lista->ultimo){
    iter->lista->ultimo = iter->ant;
  }
  if(iter->ant != NULL){
    iter->ant->prox = iter->act->prox;
  }
  nodo_t* nodo_aux = iter->act;
	void* aux = iter->act->dato;

  iter->act = iter->act->prox;
	destruir_nodo(nodo_aux);
  iter->lista->largo -=1;
	return aux;
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
  if(lista_esta_vacia(lista)){return;}
  bool temp = true;
  nodo_t* nodo_actual = lista->primero;
  while(nodo_actual != NULL) {
    if(visitar != NULL){
      temp = visitar(nodo_actual->dato,extra);
      if(temp == false){
        return;
      }
    }
    nodo_actual=nodo_actual->prox;
  }
}
