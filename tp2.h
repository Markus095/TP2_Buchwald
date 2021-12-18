#ifndef TP2_H
#define TP2_H

#include <stdbool.h>
#include <stddef.h>

typedef struct usuario usuario_t;
typedef struct post post_t;
typedef struct post_feed post_feed_t;
typedef struct red_social red_social_t;

/*
**PRE: el archivo de usuarios fue abierto y es valido
**POS: devuelve la red social con todos sus campos inicializados
*/
red_social_t* iniciar_red(FILE* archivo_usuarios);

/*
**PRE: la red social fue creada
**POS: Loggea al usuario si no habia nadie loggeado antes, en cuyo caso imprime un Error. Tambien falla si el ususario es inexistente.
*/
void logear(red_social_t* red);

/*
**PRE: la red social fue creada.
**POS: deslogguea al usuario. Si no habia usuario loggeado imprime Error.
*/
void desloggear(red_social_t* red);

/*
**PRE: la red social fue creada.
**POS: crea un post con la entrada del usuario. Si no hay nadie loggeado imprime error.
*/
void publicar(red_social_t* red);

/*
**PRE: la red social fue creada.
**POS: mostrara el siguiente post segun la afinidad del usuario. Si no hay usuario loggeado o mas posts para ver imprime error.
*/
void ver_siguiente_feed(red_social_t* red);

/*
**PRE: la red social fue creada.
**POS: aumentara los likes del post correspondiente, si el usuario no lo habia likeado antes. Si no hay usuario loggeado o no existe el post imprime error.
*/
void likear_post(red_social_t* red);

/*
**PRE: la red social fue creada.
**POS: muestra la cantidad de likes de un post. Si este no existe o no tiene likes imprime error.
*/
void mostrar_likes(red_social_t* red);

/*
**PRE: la red social fue creada.
**POS: se destruye la red junto con los posts.
*/
void destruir_red(red_social_t* red);

#endif
