/*
Apellido:Raspa, Facundo Miguel
DNI:42774804
Entrega:Si

Apellido: Rivero, Lucas Gustavo
DNI: 41765510
Entrega: Si

Apellido: Gonzalez, Esteban Gabriel
DNI: 42888511
Entrega: Si

*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "graficos.h"
#include "menus.h"



int main(int argc,char* argv[])
{
    bool corriendo = true;
    unsigned int estado = MENU;
    size_t ce;

    ///Inicializaciones

    //Ventana
    char nombreVentana[100];
    sprintf(nombreVentana, "TP Virus Simon");

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_Window *ventana = SDL_CreateWindow(nombreVentana,
                                           SDL_WINDOWPOS_CENTERED,
                                           SDL_WINDOWPOS_CENTERED,
                                           TAM_GRILLA * TAM_PIXEL * PIXELES_X_LADO  + TAM_GRILLA * PX_PADDING,
                                           TAM_GRILLA * TAM_PIXEL * PIXELES_X_LADO  + TAM_GRILLA * PX_PADDING,
                                           2);

    SDL_Renderer *renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    //Sonido
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {

        printf("[ERROR] No se pudo inicializar el audio: %s\n", Mix_GetError());
        corriendo = false;
    }


    SDL_Event evento;
    botonMenu botones_menu[CANTIDAD_BOTON_MENU];
    botonMenu botones_opciones[CANTIDAD_BOTON_OPCIONES];
    botonMenu botones_jugar[CANTIDAD_BOTON_JUGAR];
    botonMenu botones_estadistica[CANTIDAD_BOTON_ESTADISTICA];

    botonMenu *p_boton;



    while(corriendo)
    {
        if(estado == MENU)
        {
            ce= CANTIDAD_BOTON_MENU;
            menuPantalla(renderer,(SDL_Color){0,0,0,255},botones_menu,ce);
            p_boton = botones_menu;
            SDL_RenderClear(renderer);
        }
        else if(estado == JUGAR)
        {
            ce= CANTIDAD_BOTON_JUGAR;
            menuJugar(renderer,(SDL_Color){255,255,255,255},botones_jugar,ce);
            p_boton = botones_jugar;
            SDL_RenderClear(renderer);
        }
        else if(estado == OPCIONES)
        {
            ce= CANTIDAD_BOTON_OPCIONES;
            menuOpciones(renderer,(SDL_Color){100,100,100,255},botones_opciones,ce);
            p_boton = botones_opciones;
            SDL_RenderClear(renderer);

        }
        else if(estado == ESTADISTICA)
        {
            ce = CANTIDAD_BOTON_ESTADISTICA;
            menuEstadistica(renderer,(SDL_Color){200,200,200,255},botones_estadistica,ce);
            p_boton = botones_estadistica;
            SDL_RenderClear(renderer);
        }
        else if(estado == SALIR)
        {
            corriendo = false;
        }
        SDL_Delay(16);
        estado = controlEventos(&evento,p_boton,ce);
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);

    SDL_Quit();

    return 0;
}
