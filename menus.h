#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "sistemasSDL.h"
#include "constantes.h"

typedef struct
{
    SDL_Rect rectangulo;
    SDL_Color color;
    unsigned int valor_boton;
    char texto_boton[50];
    bool hover;
    SDL_Color color_texto_normal;
    SDL_Color color_texto_hover;
}tBoton;

typedef struct
{
    SDL_Point posicion;
    float transparencia;
    float velocidad;
    bool activo;
    Uint32 tiempo;
}tCrab;

typedef struct
{
    tCrab crabs[MAX_CRAB];
    Uint32 ultimo_tiempo;
    int intervalo_creacion;
}tSistemaCrab;

void mostrarPantalla(tSistemaSDL *sdl, SDL_Color color, tBoton *boton, size_t ce, tSistemaCrab *bicho, unsigned int estado);
unsigned int controlEventos(SDL_Event *evento, tBoton *botones,size_t ce,unsigned int estado_actual);
void cargarDatosBotones(tBoton *boton, size_t ce, SDL_Color *colores,int vector_valores[],char* txt[]);
void dibujarTitulo(tSistemaSDL* sdl);




#endif // MENUS_H_INCLUDED
