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

#define MENU    1
#define JUGAR   2

int main(int argc,char* argv[])
{
    bool corriendo = true;
    int estado = MENU;

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


    while(corriendo)
    {
        if(estado == MENU)
        {
            menuPantalla(renderer,(SDL_Color){255,0,0,0});
            printf("\nEstado actual: Menu Principal");
        }

        if(estado == JUGAR)
        {
            menuPantalla(renderer,(SDL_Color){0,0,255,0});
            printf("\nEstado actual: Menu Jugar");
        }


        SDL_Delay(2000);
        //SDL_Delay(16);
        corriendo = false;
    }



    return 0;
}
