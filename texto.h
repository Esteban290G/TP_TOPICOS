#ifndef TEXTO_H_INCLUDED
#define TEXTO_H_INCLUDED


#include "menus.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


void cerrarInicializacion();
void crearTexto(SDL_Renderer *rendere, botonMenu *boton, size_t ce,TTF_Font *fuente_primaria,TTF_Font *fuente_secundaria);


#endif // TEXTO_H_INCLUDED
