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
    sonidos[3] = crearTono(TONO_MAGENTA);
    sonidos[4] = crearTono(TONO_AMARILLO);
    sonidos[5] = crearTono(TONO_CIAN);
    sonidos[6] = crearTono(TONO_NARANJA);
    sonidos[7] = crearTono(TONO_MORADO);
    sonidos[8] = crearTono(TONO_ERROR);
}

void destruirTono(Mix_Chunk *tono)
{
    free(tono->abuf);
    free(tono);
}

void destruirArrayTonos(Mix_Chunk* sonidos[])
{
    for(int i = 0; i<=8; i++)
    {
        destruirTono(sonidos[i]);
    }
}
