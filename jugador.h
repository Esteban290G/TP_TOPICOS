#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "menus.h"
#include "sistemasSDL.h"
#include "graficos.h"

typedef struct
{
    char nombre[MAX_LETRAS];
    int Score;
    int modo_juego;
    int valorBoton;
}tJugador;

typedef struct
{
    char Titulo[MAX_LETRAS];
    tBoton botones[MAX_BOTONES];
    size_t ce_botones;
    SDL_Color color_fondo;
    SDL_Color color_titulo;
    SDL_Color rect_color;
    SDL_Color rect_color_borde;
    SDL_Rect rect_ingreso_nombre;
    SDL_Color color_texto_ingresado;
    char texto_ingresado[MAX_LETRAS];
    bool input_activo;
}tPantallaJugador;


void inicializarPantallaJugador(tPantallaJugador* pantalla);
void inicializarJugador(tPantallaJugador *pantalla, tJugador* jugador);

///AUX
void mostrarPantallaJuego(tSistemaSDL *sdl,tPantallaJugador* pantalla);
void mostrarTitulo(tSistemaSDL *sdl, tPantallaJugador* pantalla);
unsigned int controlEventosPantallaJuego(SDL_Event* evento, tPantallaJugador *pantalla, unsigned int estado_actual, tJugador *jugador);
void dibujarTextoIngresado(tSistemaSDL* sdl, tPantallaJugador* pantalla);

#endif // JUGADOR_H_INCLUDED
