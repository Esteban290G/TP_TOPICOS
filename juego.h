#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "sistemasSDL.h"
#include "constantes.h"
#include "graficos.h"
#include "jugador.h"

#define MEM_OK 0
#define MEM_ERROR -1
#define FILE_ERROR -2
#define FILE_OK 0
#define N_MIN 0
#define LONG_INICIAL 1

typedef struct
{
    size_t cant_botones;
    float duracion_inicial;
} tConfigJuego;

typedef struct
{
    SDL_Rect rectangulo;
    SDL_Color color_base;
    SDL_Color color_sonando;
    SDL_Color color_aux;
    unsigned int valor_boton;
    bool sonando;
    float tiempo_ultimo_sonido; // para la espera entre clics
} tBotonSimon;

typedef struct
{
    int *vecSecuencia; // vector dinamico
    int longitud;
    int indice;
    float tiempo_acumulado;
    bool mostrando;
    bool activo;
    bool reproduciendo;
    bool primera_vez;
    bool primer_boton; // bandera para que haya un tiempo de espera al inicio
    int mozart_actual; // para modo mozart
} tSecuencia;

void inicializarSecuenciaMozart(tSecuencia *secuencia, size_t cant_elem);
size_t contarElemSecuencia(char *nombre_archivo);
size_t buscarMaximo(tSecuencia *vec);
int copiarSecuenciaMozart(tSecuencia *vec, char *nombre_archivo, size_t cant_elem);
int generarAleatorio(int num_min, int num_max);
void reproducirSecuencia(tSistemaSDL *sdl, Mix_Chunk *sonidos[], tBotonSimon *boton_simon, size_t cant_botones, SDL_Color color, tBoton *boton_normal, size_t ce_normal, float deltaTime, tSecuencia *secuencia, float duracion_inicial, int limite_mozart);
void dibujarBoton(tSistemaSDL *sdl, tBotonSimon *boton, size_t cant_botones);
void dibujarPantallaJuego(tSistemaSDL *sdl, SDL_Color color, tBotonSimon *boton_simon, size_t cant_botones, tBoton *boton_normal, size_t ce_normal);
void cargarBotonSimon(tBotonSimon *boton_simon, SDL_Color *color_1, SDL_Color *color_2, size_t cant_botones, int *v_valor);
int inicializarSecuencia(tSecuencia *secuencia, size_t cant_botones);
int agregarElemSecuencia(tSecuencia *secuencia, size_t cant_botones);
bool validarJugador(tJugador *jugador, tSecuencia *sec);
void reiniciarJuego(tSecuencia *sec);


void pantalla_juego(tSistemaSDL* sdl,tJugador* jugador, bool modo, SDL_Color color);
void mostrarTexto_juego(tSistemaSDL* sdl,tJugador* jugador, bool modo);
unsigned int controlEventosPantalla_juego(SDL_Event* evento, unsigned int estado_actual, bool modo);

void colorpantalla_juego(tSistemaSDL* sdl,SDL_Color);


unsigned int controlEventosSimon(SDL_Event *evento, tBotonSimon *boton_simon, size_t cant_botones, unsigned int estado_actual, tBoton *boton_normal, size_t ce_normal, Mix_Chunk *sonidos[], tSecuencia *secuencia, float deltaTime, tJugador *jugador);

#endif // JUEGO_H_INCLUDED
