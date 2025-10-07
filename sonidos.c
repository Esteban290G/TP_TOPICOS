#include "sonidos.h"

Mix_Chunk* crearTono(float frecuencia)
{
    Mix_Chunk* tono;

    short *buffer = malloc(sizeof(short) * FREC_MUESTREO);
    if (!buffer)
    {
        printf("[Error] No se pudo reservar memoria para el tono\n");
        return NULL;
    }

    for (int i = 0; i < FREC_MUESTREO; i++)
    {
        buffer[i] = AMPLITUD_TONO * sin(2.0 * M_PI * frecuencia * i / FREC_MUESTREO);
    }

    tono = malloc(sizeof(Mix_Chunk));
    if (!tono)
    {
        printf("[Error] No se pudo reservar memoria para el tono\n");
        free(buffer);
        return NULL;
    }

    tono->alen = FREC_MUESTREO * sizeof(short); // cantidad de bytes del buffer de audio
    tono->abuf = (unsigned char*)buffer;
    tono->volume = MIX_MAX_VOLUME;

    return tono;
}

void crearArrayTonos(Mix_Chunk* sonidos[])
{
    sonidos[0] = crearTono(TONO_ROJO);
    sonidos[1] = crearTono(TONO_VERDE);
    sonidos[2] = crearTono(TONO_AZUL);
}

void destruirTono(Mix_Chunk *tono)
{
    free(tono->abuf);
    free(tono);
}

void destruirArrayTonos(Mix_Chunk* sonidos[])
{
    destruirTono(sonidos[0]);
    destruirTono(sonidos[1]);
    destruirTono(sonidos[2]);
}
