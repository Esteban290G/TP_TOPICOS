#include "graficos.h"

void colorPantalla(SDL_Renderer *renderer, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void dibujar(SDL_Renderer *renderer, botonMenu* botonRect)
{
    SDL_SetRenderDrawColor(renderer, botonRect->color.r, botonRect->color.g, botonRect->color.b, botonRect->color.a);
    SDL_RenderFillRect(renderer,&botonRect->rectangulo);
    //SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
