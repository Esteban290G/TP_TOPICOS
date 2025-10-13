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

#define FREC_DO 261.63
#define FREC_RE 293.66
#define FREC_MI 329.63
#define FREC_FA 349.23
#define FREC_SOL 392.00
#define FREC_LA 440.00
#define FREC_SI 493.88
#define FREC_DO_AGU 523.25
#define FREC_FA_SOST 369.99
#define FREC_SOL_SOST 415.30
#define FREC_LA_SOST 466.16
#define FREC_LAB 415.30
#define FREC_MIB 311.13

Mix_Chunk* crearTono(float frecuencia);
void crearArrayTonos(Mix_Chunk* sonidos[],size_t cant_botones);
void destruirTono();
void destruirArrayTonos(Mix_Chunk* sonidos[]);


#endif // SONIDOS_H_INCLUDED
