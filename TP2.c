#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "hash.h"
#include "heap.h"
#include "abb.h"

#define CAPACIDAD_INICIAL_POST 1000


typedef struct usuario{
  char* nombre;
  size_t id;
  heap_t* feed;
}usuario_t;

typedef struct post{
  char* texto;
  char* autor;
  size_t likes;
  abb_t* usuarios_likes;
  size_t id;
}post_t;

typedef struct post_feed{
  size_t afinidad;
  post_t* post;
}post_feed_t;

typedef struct red_social{
  hash_t* usuarios;
  usuario_t* usuario_loggeado;
  post_t** posts;
  size_t cantidad_posts;
  size_t cantidad_usuarios;
  size_t capacidad_post;
}red_social_t;



//////////////////////////// FUNCIONES INICIAR RED  ///////////////////////////////////////////////

void stringcpy(const char* original, char** copia) {
    size_t tam = strlen(original);
    *copia = malloc(tam + 1);

    int i;
    for(i = 0; original[i] != '\0'; i++){
      if(original[i] == '\n'){
        (*copia)[i] = '\0';
      }else{
        (*copia)[i] = original[i];
      }
    }
    (*copia)[i] = '\0';
}

int _heap_cmp(const post_feed_t* usuario1, const post_feed_t* usuario2){
  if(usuario1->afinidad > usuario2->afinidad){return -1;}
  if(usuario1->afinidad < usuario2->afinidad){return 1;}
  if(usuario1->afinidad == usuario2->afinidad){
    if(usuario1->post->id > usuario2->post->id){return -1;}
    if(usuario1->post->id < usuario2->post->id){return 1;}
  }
  return 0;
}

int heap_cmp(const void* usuario1, const void* usuario2){
  return _heap_cmp((post_feed_t*)usuario1,(post_feed_t*)usuario2);
}

usuario_t* crear_usuario(char* nombre, size_t id){
  usuario_t* usuario = calloc(1,sizeof(usuario_t));
  if(!usuario){return NULL;}
  stringcpy(nombre, &usuario->nombre);
  usuario->feed = heap_crear(heap_cmp);
  usuario->id = id;
  return usuario;
}

post_t* crear_post(char* texto, red_social_t* red){
  post_t* post = malloc(sizeof(post_t));
  stringcpy(red->usuario_loggeado->nombre, &post->autor);
  stringcpy(texto, &post->texto);
  post->likes = 0;
  post->usuarios_likes=abb_crear(strcmp,free);
  post->id = red->cantidad_posts;
  return post;
}

void destruir_post(post_t* post){
  abb_destruir(post->usuarios_likes);
  free(post->autor);
  free(post->texto);
  free(post);
}


void destruir_usuario(usuario_t* usuario){
  heap_destruir(usuario->feed, free);
  free(usuario->nombre);
  free(usuario);
}

void destruir_usuario_wrapper(void* usuario){
  destruir_usuario(usuario);
}


red_social_t* iniciar_red(FILE* archivo_usuarios){
  red_social_t* red = calloc(1,sizeof(red_social_t));
  if(!red){return NULL;}
  red->usuarios = hash_crear(destruir_usuario_wrapper);
  if(!red->usuarios){free(red); return NULL;}
  size_t capacidad = 0;
  char *linea = NULL;
  red->cantidad_usuarios = 0;
  while((getline(&linea,&capacidad,archivo_usuarios)) != EOF){
    usuario_t* usuario = crear_usuario(linea,red->cantidad_usuarios);
    hash_guardar(red->usuarios, usuario->nombre, usuario);
    red->cantidad_usuarios++;
  }
  red->usuario_loggeado = NULL;
  red->posts=malloc(CAPACIDAD_INICIAL_POST*(sizeof(post_t)));
  red->cantidad_posts = 0;
  red->capacidad_post = CAPACIDAD_INICIAL_POST;
  free(linea);
  return red;
}


void destruir_red(red_social_t* red){
  hash_destruir(red->usuarios);
  if(red->posts != NULL){
    for(size_t i=0;i < red->cantidad_posts;i++){
      destruir_post(red->posts[i]);
    }
    free(red->posts);
  }
  free(red);
}




//////////////////////////// FUNCIONES PROGRAMA INICIADO  /////////////////////////////////////////////////////////
void actualizar_feed(red_social_t* red, usuario_t* usuario_act, post_t* post){
  post_feed_t* post_feed = malloc(sizeof(post_feed_t));
  post_feed->afinidad = (size_t)abs((int)usuario_act->id - (int)red->usuario_loggeado->id);
  post_feed->post = post;
  heap_encolar(usuario_act->feed,post_feed);
}


void actualizar_feed_usuarios(red_social_t* red, post_t* post){
  hash_iter_t* iter = hash_iter_crear(red->usuarios);
  usuario_t* usuario;
  while (!hash_iter_al_final(iter)){
    usuario = hash_obtener(red->usuarios, hash_iter_ver_actual(iter));
    if(usuario->id != red->usuario_loggeado->id){
      actualizar_feed(red,usuario,post);
    }
    hash_iter_avanzar(iter);
  }
  hash_iter_destruir(iter);
}

void logear(red_social_t* red){
  if(red->usuario_loggeado != NULL){
    fprintf(stdout,"%s\n","Error: Ya habia un usuario loggeado");
    return;
  }
  size_t capacidad = 0;
  char *linea = NULL;
  getline(&linea,&capacidad,stdin);
  char* usuario_a_buscar = NULL;
  stringcpy(linea, &usuario_a_buscar);
  usuario_t* usuario = (usuario_t*)hash_obtener(red->usuarios,usuario_a_buscar);
  if(usuario != NULL){
    red->usuario_loggeado = usuario;
    fprintf(stdout,"Hola %s\n", red->usuario_loggeado->nombre);
  }else{
    fprintf(stdout,"%s\n","Error: usuario no existente");
  }
  free(usuario_a_buscar);
  free(linea);
}

void desloggear(red_social_t* red){
  if(red->usuario_loggeado == NULL){
    fprintf(stdout,"%s\n","Error: no habia usuario loggeado");
  }else{
    fprintf(stdout,"Adios\n");
    red->usuario_loggeado = NULL;
  }
}

bool redimensionar_posts(red_social_t* red){
  post_t** datos_nuevos = realloc(red->posts, red->capacidad_post * 2 * sizeof(post_t*));
  if(!datos_nuevos){
    return false;
  }
  red->capacidad_post *= 2;
  red->posts = datos_nuevos;
  return true;
}

void publicar(red_social_t* red){
  if(red->usuario_loggeado == NULL){
    fprintf(stdout,"%s\n","Error: no habia usuario loggeado");
    return;
  }
  size_t capacidad = 0;
  char *linea = NULL;
  getline(&linea,&capacidad,stdin);
  post_t* nuevo_post = crear_post(linea,red);
  red->posts[red->cantidad_posts] = nuevo_post;
  red->cantidad_posts++;
  if(red->cantidad_posts >= red->capacidad_post / 2){
    redimensionar_posts(red);
  }
  actualizar_feed_usuarios(red,nuevo_post);
  fprintf(stdout,"%s\n","Post publicado");
  free(linea);
}

void imprimir_post(post_t* post){
  fprintf(stdout, "Post ID %ld\n", post->id);
  fprintf(stdout, "%s dijo: %s\n", post->autor, post->texto);
  fprintf(stdout, "Likes: %ld\n", post->likes);
}

void ver_siguiente_feed(red_social_t* red){
  if(red->usuario_loggeado == NULL){
    fprintf(stdout,"%s\n","Usuario no loggeado o no hay mas posts para ver");
    return;
  }
  post_feed_t* siguiente = heap_desencolar(red->usuario_loggeado->feed);
  if(!siguiente){
    fprintf(stdout,"%s\n","Usuario no loggeado o no hay mas posts para ver");
    return;
  }
  imprimir_post(siguiente->post);
  free(siguiente);
}

void likear_post(red_social_t* red){
  if(red->usuario_loggeado == NULL){
    fprintf(stdout,"%s\n","Error: Usuario no loggeado o Post inexistente");
    return;
  }
  size_t capacidad = 0;
  char *linea = NULL;
  getline(&linea,&capacidad,stdin);
  int id=atoi(linea);
  if(id >= red->cantidad_posts || id < 0){
    free(linea);
    fprintf(stdout,"%s\n","Error: Usuario no loggeado o Post inexistente");
    return;
  }
  post_t* post = red->posts[id];
  if(!abb_pertenece(post->usuarios_likes, red->usuario_loggeado->nombre)){
    post->likes++;
  }
  abb_guardar(post->usuarios_likes,red->usuario_loggeado->nombre, NULL);
  fprintf(stdout,"%s\n","Post likeado");
  free(linea);
}

void imprimir_abb(abb_t* abb){
  abb_iter_t* iter = abb_iter_in_crear(abb);
  while(!abb_iter_in_al_final(iter)){
    fprintf(stdout,"	%s\n",(char*)abb_iter_in_ver_actual(iter));
    abb_iter_in_avanzar(iter);
  }
  abb_iter_in_destruir(iter);
}

void mostrar_likes(red_social_t* red){
  size_t capacidad = 0;
  char *linea = NULL;
  getline(&linea,&capacidad,stdin);
  int id= atoi(linea);
  if(id >= red->cantidad_posts|| id < 0){
    free(linea);
    fprintf(stdout,"%s\n","Error: Post inexistente o sin likes");
    return;
  }
  post_t* post = red->posts[id];
  if(post->likes == 0){
    free(linea);
    fprintf(stdout,"%s\n","Error: Post inexistente o sin likes");
    return;
  }
  fprintf(stdout,"%s %ld %s\n","El post tiene",post->likes, "likes:");
  imprimir_abb(post->usuarios_likes);
  free(linea);
}

void pedir_comando(red_social_t* red, char* linea){
  if(!linea){
    fprintf(stdout,"no se pudo leer la entrada estandar\n");
    return;
  }
  if(strcmp(linea, "login\n") == 0){
    logear(red);
  }else if(strcmp(linea, "logout\n")==0){
    desloggear(red);
  }else if(strcmp(linea, "publicar\n")==0){
    publicar(red);
  }else if(strcmp(linea, "ver_siguiente_feed\n")==0){
    ver_siguiente_feed(red);
  }else if(strcmp(linea, "likear_post\n")==0){
    likear_post(red);
  }else if(strcmp(linea, "mostrar_likes\n")==0){
    mostrar_likes(red);
  }
}




////////////////////////// MAIN /////////////////////////
int main(int argc, char* argv[]){

  if(argc < 2){
    printf("%d\n",argc);
    fprintf(stderr,"%s\n","parametros invalidos");
  }

  char *nombre_archivo = argv[1];

  FILE* archivo_usuarios = fopen(nombre_archivo,"r");
  if(!archivo_usuarios){
    fprintf(stdout,"%s\n","no se pudo abrir el archivo");
    return -1;
  }
  red_social_t* red = iniciar_red(archivo_usuarios);

  fclose(archivo_usuarios);
  
  size_t capacidad = 0;
  char *linea = NULL;
  ssize_t leidos = getline(&linea,&capacidad,stdin);
  while(leidos != EOF){
    pedir_comando(red,linea);
    leidos = getline(&linea,&capacidad,stdin);
  }
  free(linea);
  destruir_red(red);
  return 0;
}
