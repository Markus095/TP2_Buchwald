#ifndef TP2_H
#define TP2_H

#include <stdbool.h>
#include <stddef.h>

/*
**PRE:
**POS:
*/
red_social_t* iniciar_red(FILE* archivo_usuarios);

/*
**PRE:
**POS:
*/
void logear(red_social_t* red);

/*
**PRE:
**POS:
*/
void desloggear(red_social_t* red);

/*
**PRE:
**POS:
*/
void publicar(red_social_t* red);

/*
**PRE:
**POS:
*/
void ver_siguiente_feed(red_social_t* red);

/*
**PRE:
**POS:
*/
void likear_post(red_social_t* red);

/*
**PRE:
**POS:
*/
void mostrar_likes(red_social_t* red);

/*
**PRE:
**POS:
*/
void destruir_red(red_social_t* red);

#endif
