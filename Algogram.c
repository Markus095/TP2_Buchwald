#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "tp2.h"


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
  red_social_t* algogram = iniciar_red(archivo_usuarios);

  fclose(archivo_usuarios);

  size_t capacidad = 0;
  char *linea = NULL;
  ssize_t leidos = getline(&linea,&capacidad,stdin);
  while(leidos != EOF){
    pedir_comando(algogram,linea);
    leidos = getline(&linea,&capacidad,stdin);
  }
  free(linea);
  destruir_red(algogram);
  return 0;
}
