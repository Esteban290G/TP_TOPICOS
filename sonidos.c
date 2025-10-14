#include "sonidos.h"

Mix_Chunk* cargarSonido(const char *path)
{
    Mix_Chunk *sonido;

    sonido = Mix_LoadWAV(path);
    if (!sonido)
    {
        printf("Error cargando sonido \"%s:\" %s\n", path, Mix_GetError());
    }

    return sonido;
}

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

int crearArrayTonos(Mix_Chunk* sonidos[],size_t cant_botones)
{
    int tonos_creados = 0;
    float tonos[8] = {0};
    switch(cant_botones)
    {
    case 3:
    {
        float aux[] = {FREC_DO,FREC_MI,FREC_SOL};

        for(int j = 0; j < cant_botones; j++)
        {
            tonos[j] = aux[j];
        }
        tonos_creados = cant_botones;
        break;
    }
    case 4:
    {
        float aux[] = {FREC_DO,FREC_MI,FREC_SOL,FREC_LA};
        for(int j = 0; j < cant_botones; j++)
        {
            tonos[j] = aux[j];
        }
        tonos_creados = cant_botones;
        break;
    }

    case 5:
    {
        float aux[] = {FREC_DO,FREC_RE,FREC_MI,FREC_SOL,FREC_LA};
        for(int j = 0; j < cant_botones; j++)
        {
            tonos[j] = aux[j];
        }
        tonos_creados = cant_botones;
        break;
    }

    case 6:
    {
        float aux[] = {FREC_DO,FREC_RE,FREC_MI,FREC_FA_SOST,FREC_SOL_SOST,FREC_LA_SOST};
        for(int j = 0; j < cant_botones; j++)
        {
            tonos[j] = aux[j];
        }
        tonos_creados = cant_botones;
        break;
    }

    case 7:
    {
        float aux[] = {FREC_DO,FREC_RE,FREC_MIB,FREC_FA,FREC_SOL,FREC_LAB,FREC_SI};
        for(int j = 0; j < cant_botones; j++)
        {
            tonos[j] = aux[j];
        }
        tonos_creados = cant_botones;
        break;
    }
    case 8:
    {
        float aux[] = {FREC_DO,FREC_RE,FREC_MI,FREC_FA,FREC_SOL,FREC_LA,FREC_SI,FREC_DO_AGU};
        for(int j = 0; j < cant_botones; j++)
        {
            tonos[j] = aux[j];
        }
        tonos_creados = cant_botones;
        break;
    }

    }

    for(int i = 0; i < cant_botones; i++)
    {
        sonidos[i] = crearTono(tonos[i]);
    }

    return tonos_creados;
}

void destruirTono(Mix_Chunk *tono)
{
    free(tono->abuf);
    free(tono);
}


void destruirArraySonido(Mix_Chunk *sonidos[])
{
    if (sonidos == NULL)
    {
        return;
    }

    for (int i = 0; i < 8; i++)
    {
        if (sonidos[i] != NULL)
        {
            //printf("liberando\n");
            Mix_FreeChunk(sonidos[i]);
            sonidos[i] = NULL;
        }
    }
}

void destruirArrayTonos(Mix_Chunk* sonidos[], int cant_tonos)
{
    for(int i = 0; i< cant_tonos; i++)
    {
        destruirTono(sonidos[i]);
        //printf("\ntono %d libreado\n",i);
    }
}
