#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define LARGO_RECT 200
#define ANCHO_RECT 50

#define MENU    1
#define JUGAR   2
#define OPCIONES    3
#define ESTADISTICA     4
#define SALIR   5

#define JUGAR_FACIL     6
#define JUGAR_NORMAL    7
#define JUGAR_DIFICIL   8

#define OPCIONES_BOTONES    9


#define CANTIDAD_BOTON_MENU 5
#define CANTIDAD_BOTON_JUGAR 4
#define CANTIDAD_BOTON_OPCIONES 2
#define CANTIDAD_BOTON_ESTADISTICA 1


typedef struct
{
    SDL_Rect rectangulo;
    SDL_Color color;
    unsigned int valor_boton;
}botonMenu;

void menuPantalla(SDL_Renderer *renderer, SDL_Color color, botonMenu* boton,size_t ce);
void menuJugar(SDL_Renderer *renderer, SDL_Color color, botonMenu* boton, size_t ce);
void menuOpciones(SDL_Renderer *renderer, SDL_Color color, botonMenu* boton, size_t ce);
void menuEstadistica(SDL_Renderer *renderer, SDL_Color color, botonMenu *boto,size_t ce);
unsigned int controlEventos(SDL_Event *evento,const botonMenu *botones,size_t ce);



#endif // MENUS_H_INCLUDED
