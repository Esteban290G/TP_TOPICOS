#include "graficos.h"

void colorPantalla(SDL_Renderer *renderer, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void dibujar(SDL_Renderer *renderer, botonMenu* botonRect, size_t ce)
{
    for(botonMenu* i = botonRect; i < botonRect + ce; i ++)
    {
        SDL_SetRenderDrawColor(renderer, i->color.r, i->color.g, i->color.b, i->color.a);
        SDL_RenderFillRect(renderer,&i->rectangulo);

    }
    SDL_RenderPresent(renderer);
}
