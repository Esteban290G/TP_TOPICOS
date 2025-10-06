#ifndef CRAB_H_INCLUDED
#define CRAB_H_INCLUDED
#include "constantes.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct
{
    SDL_Point posicion;
    float transparencia;
    float velocidad;
    bool activo;
    Uint32 tiempo;
}tCrab;

typedef struct
{
    tCrab crabs[]
}


#endif // CRAB_H_INCLUDED
