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
**POS: pide el nombre del usuario que se va a loggear, si no había nadie loggeado
** y el nombre es valido, lo loggea. De lo contrario, imprime un error por entrada
** estandar.
*/
void logear(red_social_t* red);

/*
**PRE: la red social fue creada.
**POS: deslogguea al usuario loggeado actualmente. Si no hay tal usuario imprime
** un error por la entrada estandar.
*/
void desloggear(red_social_t* red);

/*
**PRE: la red social fue creada.
**POS: si hay un usuario loggeado, pide el texto del post a publicar, lo crea y
** lo guarda en la red y los feeds de todos los usuarios menos el loggeado.
** En el caso contrario imprime un error por la entrada estandar.
*/
void publicar(red_social_t* red);

/*
**PRE: la red social fue creada.
**POS: mostrara el texto, autor y cantidad de likes del siguiente post en el
** feed del usuario y lo quita del feed. El siguiente post se determina en base
** a que tan cerca estaban el autor del post y el usuario loggeado en el archivo original.
**Si no hay usuario loggeado o mas posts para ver imprime un error por entrada estandar.
*/
void ver_siguiente_feed(red_social_t* red);

/*
**PRE: la red social fue creada.
**POS: pide el id del post, si el post con ese id existe y hay un usuario loggeado,
** aumenta los likes del post, guarda al usuario actualmente loggeado entre
** los que le dieron like e imprime "Post likeado". Si el usuario ya habia dado
** like al post previamente, solamente imprime el mensaje.
**Si no hay usuario loggeado o no existe el post imprime error.
*/
void likear_post(red_social_t* red);

/*
**PRE: la red social fue creada.
**POS: pide el id del post. Si este no existe o no tiene likes imprime error.
** En caso contrario, imprime la cantidad de likes y el nombre de todos usuarios
** que previamente lo likearon.
*/
void mostrar_likes(red_social_t* red);

/*
**PRE: la red social fue creada.
**POS: se destruye la red junto con los posts, los usuarios y sus feeds.
*/
void destruir_red(red_social_t* red);

#endif
