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
    SDL_Color color;
    SDL_Color color_apretado;
    SDL_Color color_hover;
    SDL_Rect rectangulo;
    bool apretado;
    bool hover;
}tBoton_fondo;

typedef struct
{
    SDL_Rect rectangulo;
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



void mostrarPantalla(tSistemaSDL *sdl, SDL_Color color, tBoton *boton, size_t ce_normal, tBoton_fondo *boton_fondo, size_t ce_fondo ,unsigned int estado);
unsigned int controlEventos(SDL_Event *evento, tBoton *botones,size_t ce,tBoton_fondo* boton_fondo,unsigned int estado_actual);
void dibujarTitulo(tSistemaSDL* sdl);

void cargarDatosBotones(tBoton *boton, size_t ce, SDL_Color *colores,int vector_valores[],char* txt[]);

///Por si necesitamos sacarlo o realizar modificaciones

void dibujarFondo(tSistemaSDL *sdl, tBoton_fondo* boton_fondo, size_t ce_fondo);
void inicializarBoton_fondo(tBoton_fondo *boton_fondo, SDL_Color* colores, SDL_Color* color_hover, SDL_Color* color_apretado);


#endif // MENUS_H_INCLUDED
