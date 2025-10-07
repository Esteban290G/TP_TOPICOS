#ifndef GRAFICOS_H_INCLUDED
#define GRAFICOS_H_INCLUDED

#include <SDL2/SDL.h>
#include <time.h>
#include <math.h>
#include "menus.h"
#include "sistemasSDL.h"
#include "constantes.h"



bool _verificarMouseBoton(SDL_Rect boton, int mouse_x, int mouse_y);
void colorPantalla(tSistemaSDL* sdl, SDL_Color color);
void dibujar(tSistemaSDL* sdl, tBoton* botonRect, size_t ce);
void renderizarTexto(tSistemaSDL* sdl,tBoton* boton);

void inicializartSistemaCrab(tSistemaCrab* bicho);
void crearCrab(tSistemaCrab* bicho, int ancho_pantalla, int largo_pantalla);
void actualizarCrab(tSistemaCrab* bicho);
void dibujarCrab(tSistemaSDL *sdl,tSistemaCrab* bicho);


#endif // GRAFICOS_H_INCLUDED
