#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "heap.h"


#define CAPACIDAD_INICIAL 20


void heap_sort(void *elementos[], size_t tope, cmp_func_t cmp){
  heap_t* heap=heap_crear(cmp);
  if (!heap) return;
  for (size_t i=0; i<tope; i++){
    heap_encolar(heap,elementos[i]);
  }for (size_t i=0; i<tope; i++){
    elementos[tope-1-i]=heap_desencolar(heap);
  }
  heap_destruir(heap,NULL);
}

/* Tipo utilizado para el heap. */
struct heap{
  void** datos;
  size_t cantidad;
  size_t capacidad;
  cmp_func_t cmp;
};
/////////////FUNCIONES AUXILIARES//////////////////
void swap(void** x, void** y){
  void* aux=*x;
  *x=*y;
  *y=aux;
}

void downheap(heap_t* heap, size_t pos, size_t n){
  if(pos >= n){return;}
  size_t min=pos;
  size_t izq=2*pos+1;
  size_t der=2*pos+2;
  if(izq < n && heap->cmp(heap->datos[izq],heap->datos[min]) > 0){min=izq;}
  if(der < n && heap->cmp(heap->datos[der],heap->datos[min]) > 0){min=der;}
  if(min != pos){
    swap(&heap->datos[min],&heap->datos[pos]);
    downheap(heap,min, n);
  }
}

void _heapify(heap_t* heap, size_t pos){
  if(pos >= heap->cantidad/2){return;}
  _heapify(heap,2*pos+1);
  _heapify(heap,2*pos+2);
  downheap(heap,pos, heap->cantidad);
}

void heapify(heap_t* heap){
  _heapify(heap,0);
}

void upheap(heap_t* heap, size_t pos){
  if(pos == 0){return;}
  size_t padre=(pos-1)/2;
  if(heap->cmp(heap->datos[padre],heap->datos[pos]) < 0){
    swap(&heap->datos[padre],&heap->datos[pos]);
    upheap(heap,padre);
  }
}

bool heap_redimensionar(heap_t* heap){
  size_t nueva_capacidad = 0;
  if(heap->cantidad == heap->capacidad){
    nueva_capacidad=heap->capacidad*2;
  }else if(heap->cantidad <= heap->capacidad/4){
    nueva_capacidad=heap->capacidad/2;
  }
  void* nuevo_datos=realloc(heap->datos,nueva_capacidad*(sizeof(void*)));
  if(!nuevo_datos){return false;}
  heap->datos=nuevo_datos;
  heap->capacidad=nueva_capacidad;
  return true;
}


//////////////////PRIMITIVAS DEL TDA//////////////////////////////

heap_t *heap_crear(cmp_func_t cmp){
  heap_t* heap=malloc(sizeof(heap_t));
  if(!heap){return NULL;}
  void** datos=malloc(CAPACIDAD_INICIAL*sizeof(void*));
  if(!datos){
    free(heap);
    return NULL;
  }
  heap->datos=datos;
  heap->cantidad=0;
  heap->capacidad=CAPACIDAD_INICIAL;
  heap->cmp=cmp;
  return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
  heap_t* heap=malloc(sizeof(heap_t));
  if(!heap){return NULL;}
  void** datos=malloc((n+CAPACIDAD_INICIAL)*sizeof(void*));
  if(!datos){
    free(heap);
    return NULL;
  }
  for(size_t i=0; i < n;i++){datos[i] = arreglo[i];}
  heap->datos=datos;
  heap->cantidad=n;
  heap->capacidad=n+CAPACIDAD_INICIAL;
  heap->cmp=cmp;
  heapify(heap);
  return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){
  if(destruir_elemento != NULL){
    for(size_t i=0;i < heap->cantidad;i++){
      destruir_elemento(heap->datos[i]);
    }
  }
  free(heap->datos);
  free(heap);
}

size_t heap_cantidad(const heap_t *heap){
  return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
  return heap->cantidad == 0;
}

bool heap_encolar(heap_t *heap, void* elemento){
  if(heap->cantidad == heap->capacidad){
    if(!heap_redimensionar(heap)){return false;}
  }
  heap->datos[heap->cantidad] = elemento;
  if(heap->cantidad > 0){upheap(heap,heap->cantidad);}
  heap->cantidad++;
  return true;
}

void *heap_ver_max(const heap_t *heap){
  if(heap_esta_vacio(heap)){return NULL;}
  return heap->datos[0];
}

void *heap_desencolar(heap_t *heap){
  if(heap_esta_vacio(heap)){return NULL;}
  if(heap->cantidad <= heap->capacidad/4 && heap->capacidad >= CAPACIDAD_INICIAL){
    if(!heap_redimensionar(heap)){
      return NULL;
    }
  }
  void* elemento = heap->datos[0];
  heap->datos[0] = heap->datos[heap->cantidad-1];
  downheap(heap,0, heap->cantidad-1);
  heap->cantidad--;
  heap->datos[heap->cantidad]=NULL;
  return elemento;
}
