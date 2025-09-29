#ifndef GRAFICOS_H_INCLUDED
#define GRAFICOS_H_INCLUDED

#include <SDL2/SDL.h>
#include "menus.h"

#define TAM_PIXEL 10
#define PIXELES_X_LADO 12
#define TAM_GRILLA 6
#define PX_PADDING 4

void colorPantalla(SDL_Renderer *renderer, SDL_Color color);
void dibujar(SDL_Renderer *renderer, botonMenu* botonRect, size_t ce);



#endif // GRAFICOS_H_INCLUDED
