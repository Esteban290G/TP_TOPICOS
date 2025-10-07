#ifndef SONIDOS_H_INCLUDED
#define SONIDOS_H_INCLUDED
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <math.h>

#define FREC_MUESTREO 44100
#define BUFFER_AUDIO 1024
#define AMPLITUD_TONO 8192

// Frecuencias tonos
#define TONO_ROJO 440
#define TONO_VERDE 450
#define TONO_AZUL 460

Mix_Chunk* crearTono(float frecuencia);
void crearArrayTonos(Mix_Chunk* sonidos[]);
void destruirTono();
void destruirArrayTonos(Mix_Chunk* sonidos[]);


#endif // SONIDOS_H_INCLUDED
