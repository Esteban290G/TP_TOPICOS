#ifndef OPCIONES_H_INCLUDED
#define OPCIONES_H_INCLUDED

#include "constantes.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "estadistica.h"
#include "menus.h"
#include "juego.h"


typedef struct
{
    SDL_Rect rectangulo;
    char texto[MAX_LETRAS];
    bool seleccionado;
}tOpciones_sonidos;

typedef struct
{
    SDL_Rect boton_despregable;
    char texto[MAX_LETRAS];
    bool abierto;
    tOpciones_sonidos opciones[3];
    int cantidad_opciones;
}tOpciones_despregable;

typedef struct
{
    char Titulo[MAX_LETRAS];
    SDL_Color color_letras;
    tBoton_triangular boton_triangular[4];
    char texto_botones[MAX_LETRAS];
    char texto_velocidad[MAX_LETRAS];
    char texto_sonidos[MAX_LETRAS];
    SDL_Rect rectangulo[3];
    tBoton boton[CANTIDAD_BOTON_OPCIONES];
    SDL_Color fondo;
    tConfigJuego config;
    tOpciones_despregable menu_sonido;
}tOpciones;


void inicializarOpciones(tOpciones *op);
void mostrarPantallaOpciones(tSistemaSDL* sdl, tOpciones* op);
void mostrarTituloOpciones(tSistemaSDL* sdl, tOpciones* op);
void mostrarTextos(tSistemaSDL* sdl, tOpciones* op);
void mostrarTextosopciones(tSistemaSDL* sdl, tOpciones * op,char*opciones);
unsigned int eventosOpciones(SDL_Event* evento, tOpciones *op, unsigned int estado_actual, tConfigJuego *configJuego);
void dibujarMenuDespregable(tSistemaSDL* sdl, tOpciones_despregable* menu_despregable);


void dibujarfondodegrade(tSistemaSDL*sdl);
void dibujarLineasSeparadoras(tSistemaSDL*sdl, int y);

#endif // OPCIONES_H_INCLUDED
