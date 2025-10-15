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
    }
}

void cargarSonidosCounter(Mix_Chunk* armas[])
{
    Mix_Chunk *counter_01 = cargarSonido("snd/counter_01.wav");
    Mix_Chunk *counter_02 = cargarSonido("snd/counter_02.wav");
    Mix_Chunk *counter_03 = cargarSonido("snd/counter_03.wav");
    Mix_Chunk *counter_04 = cargarSonido("snd/counter_04.wav");
    Mix_Chunk *counter_05 = cargarSonido("snd/counter_05.wav");
    Mix_Chunk *counter_06 = cargarSonido("snd/counter_06.wav");
    Mix_Chunk *counter_07 = cargarSonido("snd/counter_07.wav");
    Mix_Chunk *counter_08 = cargarSonido("snd/counter_08.wav");

    armas[0] = counter_01;
    armas[1] = counter_02;
    armas[2] = counter_03;
    armas[3] = counter_04;
    armas[4] = counter_05;
    armas[5] = counter_06;
    armas[6] = counter_07;
    armas[7] = counter_08;
}

void cargarSonidosMinecraft(Mix_Chunk* mine[])
{
    Mix_Chunk *mine_01 = cargarSonido("snd/minecraft_01.wav");
    Mix_Chunk *mine_02 = cargarSonido("snd/minecraft_02.wav");
    Mix_Chunk *mine_03 = cargarSonido("snd/minecraft_03.wav");
    Mix_Chunk *mine_04 = cargarSonido("snd/minecraft_04.wav");
    Mix_Chunk *mine_05 = cargarSonido("snd/minecraft_05.wav");
    Mix_Chunk *mine_06 = cargarSonido("snd/minecraft_06.wav");
    Mix_Chunk *mine_07 = cargarSonido("snd/minecraft_07.wav");
    Mix_Chunk *mine_08 = cargarSonido("snd/minecraft_08.wav");

    mine[0] = mine_01;
    mine[1] = mine_02;
    mine[2] = mine_03;
    mine[3] = mine_04;
    mine[4] = mine_05;
    mine[5] = mine_06;
    mine[6] = mine_07;
    mine[7] = mine_08;
}
