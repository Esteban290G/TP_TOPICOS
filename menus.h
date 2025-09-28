#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED

#include <SDL2/SDL.h>

#define LARGO_RECT 200
#define ANCHO_RECT 50

typedef struct
{
    SDL_Rect rectangulo;
    SDL_Color color;
}botonMenu;

void menuPantalla(SDL_Renderer *renderer, SDL_Color color);


#endif // MENUS_H_INCLUDED
