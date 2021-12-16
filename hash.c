#define _POSIX_C_SOURCE 200809L
#include "hash.h"
#include "lista.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define CAPACIDAD_INICIAL 50
#define FACTOR_DE_CARGA 2
#define FACTOR_REDIMENSION 2

typedef struct hash_campo {
	char *clave;
    void *valor;
} hash_campo_t;

struct hash {
    lista_t **tabla;
    size_t cantidad;
    size_t capacidad;
    hash_destruir_dato_t destruir_dato;
};

struct hash_iter{
	const hash_t* hash;
	lista_iter_t* iter_lista;
	size_t posicion_lista_actual;
	size_t cantidad_datos;
};

size_t funcion_hashing(const char* clave,size_t capacidad){   
    size_t hash = 5381;
    int c;
    while ((c = *clave++))
        hash = ((hash << 5) + hash) + (size_t)(c);
    return hash % capacidad;
}

bool hash_redimensionar(hash_t* hash,size_t nueva_capacidad){
	lista_t** tabla = malloc(sizeof(lista_t*)*nueva_capacidad);
	if (nueva_capacidad > 0 && !tabla) {return false;}
	for (int i = 0; i < nueva_capacidad; ++i){
		tabla[i] = lista_crear();
	}
	for (int i = 0; i < hash->capacidad; ++i){		
		while(!lista_esta_vacia(hash->tabla[i])){
			hash_campo_t* hash_campo = lista_borrar_primero(hash->tabla[i]);
			size_t posicion = funcion_hashing(hash_campo->clave,nueva_capacidad);
			lista_insertar_ultimo(tabla[posicion],hash_campo);
		}
		lista_destruir(hash->tabla[i],NULL);
	}
	free(hash->tabla);
	hash->tabla = tabla;
	hash->capacidad = nueva_capacidad;
	return true;
}

hash_campo_t* hash_campo_crear(const char* clave, void* valor){
	hash_campo_t* hash_campo = malloc(sizeof(hash_campo_t));
	if (!hash_campo) {return NULL;}
	char* clave_aux = strdup(clave);
	hash_campo->clave = clave_aux;
	hash_campo->valor = valor;
	return hash_campo;
}

hash_t* hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if (!hash) {return NULL;}
	lista_t** tabla = malloc(sizeof(lista_t*)*CAPACIDAD_INICIAL);
	if (CAPACIDAD_INICIAL > 0 && !tabla){
		free(hash);
		return NULL;
	} 
	for (int i = 0; i < CAPACIDAD_INICIAL; ++i){
		tabla[i] = lista_crear();
	}
	hash->tabla =  tabla;
	hash->cantidad = 0;
	hash->capacidad = CAPACIDAD_INICIAL;
	hash->destruir_dato = destruir_dato;
	return hash;
}

hash_campo_t* hash_buscar(const hash_t* hash, const char* clave,bool borrar){
	size_t posicion = funcion_hashing(clave,hash->capacidad);
	lista_t* lista = hash->tabla[posicion];
	if (lista_esta_vacia(lista)){ return NULL;}
	lista_iter_t* iter = lista_iter_crear(lista);
	while(!lista_iter_al_final(iter) ){
		hash_campo_t* actual = lista_iter_ver_actual(iter);
		if (strcmp(clave,actual->clave) == 0){
			if (borrar){ lista_iter_borrar(iter);}
			lista_iter_destruir(iter);
			return actual;
		} 
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return NULL;
}

bool hash_guardar( hash_t *hash, const char *clave, void *dato){
	if (hash->cantidad/hash->capacidad >= FACTOR_DE_CARGA ){
		if (!hash_redimensionar(hash,hash->capacidad*FACTOR_REDIMENSION)) {return false;}
	}
	const hash_t* hash_aux = hash;
	hash_campo_t* hash_campo_aux = hash_buscar(hash_aux,clave,false);
	if (!hash_campo_aux){
		hash_campo_t* hash_campo = hash_campo_crear(clave,dato);
		if (!hash_campo) {return false;}
		size_t posicion = funcion_hashing(clave,hash->capacidad);
		lista_insertar_ultimo(hash->tabla[posicion],hash_campo);
		hash->cantidad++;
	} 
	else{
		void* valor_aux = hash_campo_aux->valor;
		hash_campo_aux->valor = dato;
		if(hash->destruir_dato) {hash->destruir_dato(valor_aux);}
		
	} 
	return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
	if (hash_cantidad(hash) == 0) {return NULL;}
	const hash_t* hash_aux = hash;
	hash_campo_t* hash_campo = hash_buscar(hash_aux,clave,true);
	if (!hash_campo) {return NULL;}
	void* valor_aux = hash_campo->valor; 
	free(hash_campo->clave);
	free(hash_campo);
	hash->cantidad--;
	return valor_aux;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	if (hash_cantidad(hash) == 0) {return NULL;}
	hash_campo_t* hash_campo = hash_buscar(hash,clave,false);
	if (!hash_campo) {return NULL;}
	return hash_campo->valor;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	if (hash_cantidad(hash) == 0){return false;}
	hash_campo_t* hash_campo = hash_buscar(hash,clave,false);
	if (!hash_campo) {return false;}
	return true;
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cantidad;
}

void hash_destruir(hash_t *hash){
	for (size_t i = 0; i < hash->capacidad; ++i){
		while(!lista_esta_vacia(hash->tabla[i])){
			hash_campo_t* hash_campo = lista_borrar_primero(hash->tabla[i]);
			if (hash->destruir_dato != NULL){ hash->destruir_dato(hash_campo->valor);}
			free(hash_campo->clave);
			free(hash_campo);
			}
		free(hash->tabla[i]);
	}
	free(hash->tabla);
	free(hash);
}

/* FUNCION AUXILIAR PARA EL ITERADOR */

lista_t* buscar_lista_no_vacio(hash_iter_t* iter){
	if(iter->posicion_lista_actual > 0) {iter->posicion_lista_actual++;}
	while(iter->posicion_lista_actual < iter->hash->capacidad){
		if (!lista_esta_vacia(iter->hash->tabla[iter->posicion_lista_actual])){
			return iter->hash->tabla[iter->posicion_lista_actual];
		}
		iter->posicion_lista_actual++;
	}
	return NULL;
}


/* PRIMITIVAS DEL ITERADOR */

hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if (!iter) {return NULL;}
	iter->hash = hash;
	iter->posicion_lista_actual = 0;
	iter->cantidad_datos = 0;
	iter->iter_lista = NULL;
	lista_t* lista = buscar_lista_no_vacio(iter);
	if(lista) {iter->iter_lista = lista_iter_crear(lista);}
	
	return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
	if (hash_iter_al_final(iter)) {return false;}
	lista_iter_avanzar(iter->iter_lista);
	if (lista_iter_al_final(iter->iter_lista)){
		lista_t* lista = buscar_lista_no_vacio(iter);
		if(lista) {
			lista_iter_destruir(iter->iter_lista);
			iter->iter_lista = lista_iter_crear(lista);
		}
	}
	iter->cantidad_datos++;
	return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if (hash_iter_al_final(iter)) {return false;}
	hash_campo_t* hash_campo = lista_iter_ver_actual(iter->iter_lista);
	return hash_campo->clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){
	return (iter->posicion_lista_actual == iter->hash->capacidad || (iter->cantidad_datos) == iter->hash->cantidad);
}

void hash_iter_destruir(hash_iter_t* iter){
	if(iter->iter_lista){lista_iter_destruir(iter->iter_lista);}
	free(iter);
}