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
#define TONO_ERROR 100.0
#define TONO_ROJO 261.63 //DO
#define TONO_VERDE 329.63 //MI
#define TONO_AZUL 392.00 //SOL

Mix_Chunk* crearTono(float frecuencia);
void crearArrayTonos(Mix_Chunk* sonidos[]);
void destruirTono();
void destruirArrayTonos(Mix_Chunk* sonidos[]);


#endif // SONIDOS_H_INCLUDED
