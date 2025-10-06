#include "texto.h"

void crearTexto(SDL_Renderer *rendere, botonMenu *boton, size_t ce,TTF_Font *fuente_primaria,TTF_Font *fuente_secundaria)
{
    SDL_Color blanco = {255,255,255,255};
    SDL_Color amarillo = {255,255,0,255};
    SDL_Surface *normall = TTF_RenderText_Solid(fuente_primaria, boton->texto,blanco);
    boton->normal = SDL_CreateTexture(rendere,normall);
    SDL_FreeSurface(normal);

    SDL_Surface *hover = TTF_RenderText_Solid(fuente_secundaria,boton->texto,amarillo);
    boton->hover = SDL_CreateTexture(rendere,hover);
    SDL_FreeSurface(hover);
}

void cerrarInicializacion(TTF_Font fuente_principal, TTF_Font fuente_secundaria)
{
    TTF_CloseFont(fuente_principal);
    TTF_CloseFont(fuente_secundaria);
    TTF_Quit();
}

