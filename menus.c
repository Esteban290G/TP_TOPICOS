#include "menus.h"
#include "graficos.h"

void menuPantalla(SDL_Renderer *renderer, SDL_Color color)
{
    colorPantalla(renderer, color);

    botonMenu boton;
    boton.color.r = 0;
    boton.color.g = 255;
    boton.color.b = 0;
    boton.color.a = 255;

    /* boton.rectangulo.x = 40;
    boton.rectangulo.y = 40;
    boton.rectangulo.w = 30;
    boton.rectangulo.h = 15; */

    boton.rectangulo = (SDL_Rect){275,400,LARGO_RECT,ANCHO_RECT};
    dibujar(renderer,&boton);
}
