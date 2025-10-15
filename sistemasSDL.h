#ifndef SISTEMASSDL_H_INCLUDED
#define SISTEMASSDL_H_INCLUDED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

typedef struct
{
    SDL_Window* ventana;
    SDL_Renderer* renderer;
    TTF_Font* fuente;
    TTF_Font* fuente2;
    TTF_Font* fuente_titulo;
    bool audio_inicializado;
    bool imagen_inicializada;
    Mix_Music* musica_menu;
}tSistemaSDL;

void inicializarNULL(tSistemaSDL* sistema);
bool inicializarSDL(tSistemaSDL* sistema,const char* nombre_ventana, int ancho, int largo);
void limpiarSDL(tSistemaSDL* sistema);
void reproducirMusica(tSistemaSDL* sdl);
void pausarMusica(tSistemaSDL* sdl);
void reanudarMusica(tSistemaSDL* sdl);


#endif // SISTEMASSDL_H_INCLUDED
