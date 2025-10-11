#ifndef ESTADISTICA_H_INCLUDED
#define ESTADISTICA_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "constantes.h"
#include "menus.h"
#include "jugador.h"
#include "sistemasSDL.h"

#define ARCHIVO_CREADO -1

#define MAX_JUGADORES 5

//estadistica.dat
//estadisticamo.dat



typedef struct
{
    SDL_Point posicion;
    SDL_Color color_relleno;
    SDL_Color color_bordes;
    unsigned int estado;
}tBoton_triangular;

typedef struct
{
    char titulo[MAX_LETRAS];
    tBoton botones[CANTIDAD_BOTON_ESTADISTICA];
    SDL_Rect rec_estadistica;
    tJugador jugador[MAX_JUGADORES];
    tJugador jugador_mo[MAX_JUGADORES];
    size_t ce_jugadores;
    size_t ce_jugadores_mo;
    SDL_Color fondo;
    SDL_Color rec_fondo;
    SDL_Color rec_bordes;
    SDL_Color color_titulo;
    SDL_Color color_texto;
    size_t ce_botones_normales;
    bool es_schon;
    tBoton_triangular botones_triangulares[2];
    char texto_est[MAX_LETRAS];
}tEstadistica;


///Logica para crear y agregar al archivo
///Logica para ordenar el vector de los puntos

FILE* corroborarArchivo(char* texto);
int agregarDatosVector(tJugador *jugador, char* texto);
void insertarVectorOrdenado(tJugador *jugador, size_t* ce, tJugador nuevo);
bool agregarArchivo(tJugador *jugador, size_t ce, char* texto);


///Parte visual de la estadistica

void inicializarPantallaEstadistica(tEstadistica* estadistica);
void mostrarPantallaEstadistica(tSistemaSDL*sdl ,tEstadistica* estadistica);
void mostrarTituloEstadistica(tSistemaSDL* sdl, tEstadistica* estadistica);
unsigned int controlEventosEstadistica(SDL_Event* eventos, tEstadistica* estadistica, unsigned int estado_actual);
void mostrarJugadores_mo(tSistemaSDL* sdl, tEstadistica* estadistica);
void mostrarJugadores(tSistemaSDL* sdl, tEstadistica* estadistica);
void mostrarTextoEst(tSistemaSDL* sdl, tEstadistica* estadistica, char* texto_est);

///Logica botones triangulares
void dibujarBotonesTriangulares(tSistemaSDL *sdl, tBoton_triangular* botones, size_t ce);
void invertirMatriz(const int original[][12], int destino[][12], int direccion);

#endif // ESTADISTICA_H_INCLUDED
