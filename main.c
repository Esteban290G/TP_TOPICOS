/*
Apellido: Raspa, Facundo Miguel
DNI: 42774804
Entrega: Si

Apellido: Rivero, Lucas Gustavo
DNI: 41765510
Entrega: Si

Apellido: Gonzalez, Esteban Gabriel
DNI: 42888511
Entrega: Si

*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "graficos.h"
#include "menus.h"
#include "sistemasSDL.h"
#include "constantes.h"
#include "juego.h"
#include "sonidos.h"
#include "jugador.h"
#include "estadistica.h"
#include "opciones.h"



int main(int argc,char* argv[])
{
    //Variables para iniciar el programa
    bool corriendo = true;
    unsigned int estado = MENU;
    char nombreVentana[] = "TP Virus Simon";

    //Variables para el tiempo
    Uint32 tiempoAnterior = SDL_GetTicks();
    float deltaTime = 0;

    //Inicializar SDL
    tSistemaSDL sdl;

    if(!inicializarSDL(&sdl,nombreVentana,ANCHO,LARGO))
    {
        printf("Error, no se pudo inicializar SDL\n");
        return 1;
    }



    ///reproducirMusica(&sdl);

    // Cargar y generar los sonidos
    Mix_Chunk* sonidos[8];
    crearArrayTonos(sonidos);

    // Estructura secuencia
    tSecuencia sec;

    //  Colores Rojo - Verde - Azul - Magenta - Amarillo - Cian - Naranja - Morado
    SDL_Color colores[8] = {{191,0,0,255},{0,191,0,255},{0,0,191,255},{191,0,191,255},{191,191,0,255},{0,191,191,255},{191,124,0,255},{65,26,75,255}};
    SDL_Color colores_luz[8] = {{236,83,83,255},{83,236,83,255},{83,83,236,255},{236,83,236,255},{236,236,83,255},{83,236,236,255},{236,182,83,255},{104,52,117,255}};
    SDL_Color fondo = (SDL_Color)
    {
        115,115,115,128
    };

    SDL_Color colores_hover[] = {{255, 70, 70, 255},{70, 255, 70, 255},{70, 70, 255, 255},{255, 70, 255, 255},{255, 255, 70, 255},{70, 255, 255, 255},{255, 190, 70, 255},{120, 60, 130, 255}};

    SDL_Color colores_apretado[] = {{100, 0, 0, 255},{0, 100, 0, 255},{0, 0, 100, 255},{100, 0, 100, 255},{120, 120, 0, 255},{0, 100, 100, 255},{120, 70, 0, 255},{30, 10, 40, 255}};

    // Estructuras para el juego
    SDL_Event evento;
    tPantallaJugador pantalla_jugador;
    tJugador jugador;
    tEstadistica pantalla_estadistica;
    tOpciones opciones;

    inicializarPantallaJugador(&pantalla_jugador);
    inicializarJugador(&pantalla_jugador,&jugador);
    inicializarPantallaEstadistica(&pantalla_estadistica);
    inicializarOpciones(&opciones);

    tSistemaCrab bicho_crab;
    inicializartSistemaCrab(&bicho_crab);

    cargarTopDesdeArchivo(pantalla_estadistica.jugador, &pantalla_estadistica.ce_jugadores, "top_schonberg.dat");
    cargarTopDesdeArchivo(pantalla_estadistica.jugador_mo, &pantalla_estadistica.ce_jugadores_mo, "top_mozart.dat");

    // Datos de botones y texto
    int vector_valores_menu[] = {JUGAR, OPCIONES, ESTADISTICA, SALIR};
    //int vector_valores_opciones[] = {OPCIONES_BOTONES, MENU};
    int vector_valores_estadistica[] = {MENU};
    int vector_valores_jugar[] = {SCHONBERG,MOZART,MENU};
    int vector_valores_simon[] = {BOTON_1,BOTON_2,BOTON_3,BOTON_4,BOTON_5,BOTON_6,BOTON_7,BOTON_8};
    int vector_valores_fondo[] = {BOTON_111,BOTON_112,BOTON_113,BOTON_114,BOTON_115,BOTON_116,BOTON_117,BOTON_118};
    int vector_valores_aux_simon[] = {JUGAR,SALIR}; ///AUX

    char* texto_menu[] = {"jugar","opciones","estadistica","salir"};
    //char* texto_opciones[] = {"Modificar","Volver"};
    char* texto_estadistica[] = {"Volver"};
    char* texto_jugar[] = {"Modo Schonberg", "Modo Mozart", "Volver"};
    char* texto_aux_simon[] = {"Volver","Salir"}; ///AUX

    // Cargar datos a los botones
    tBoton botones_menu[CANTIDAD_BOTON_MENU];
    cargarDatosBotones(botones_menu,CANTIDAD_BOTON_MENU,colores, vector_valores_menu,texto_menu);

    //tBoton botones_opciones[CANTIDAD_BOTON_OPCIONES];
    //cargarDatosBotones(botones_opciones, CANTIDAD_BOTON_OPCIONES, colores,vector_valores_opciones,texto_opciones);

    tBoton botones_estadistica[CANTIDAD_BOTON_ESTADISTICA];
    cargarDatosBotones(botones_estadistica,CANTIDAD_BOTON_ESTADISTICA,colores,vector_valores_estadistica,texto_estadistica);

    tBoton botones_jugar[CANTIDAD_BOTON_JUGAR];
    cargarDatosBotones(botones_jugar,CANTIDAD_BOTON_JUGAR,colores,vector_valores_jugar,texto_jugar);

    ///AUX
    tBoton botones_aux_simon[2];
    cargarDatosBotones(botones_aux_simon,2,colores,vector_valores_aux_simon,texto_aux_simon);
    botones_aux_simon[0].rectangulo.x = 10;
    botones_aux_simon[0].rectangulo.y = 10;
    botones_aux_simon[1].rectangulo.x = 500;
    botones_aux_simon[1].rectangulo.y = 10;

    // Cargamos los datos al boton_simon
    tBotonSimon boton_simon[8];

    tConfigJuego configuracion;
    configuracion.cant_botones = 3;
    configuracion.duracion_inicial = 2000;
    bool cargar_botones = true;

    ///AUX FONDO

    tBoton_fondo boton_fondo[8];

    inicializarBoton_fondo(boton_fondo, colores,colores_hover,colores_apretado);

    ///

    tConfeti confeti[MAX_CONFETI];
    inicializarConfeti(confeti);

    bool modo = true;
    bool win = false;

    while(corriendo)
    {
        Uint32 tiempoActual = SDL_GetTicks();
        deltaTime = tiempoActual - tiempoAnterior;
        tiempoAnterior = tiempoActual;
        //estado = MOZART;
        switch(estado)
        {
        case MENU:
            estado = controlEventos(&evento,botones_menu,CANTIDAD_BOTON_MENU,boton_fondo,estado);
            mostrarPantalla(&sdl,(SDL_Color){0,0,0,255},botones_menu,CANTIDAD_BOTON_MENU,boton_fondo,(size_t)8,estado);
            break;

        case JUGAR:
            ///reanudarMusica(&sdl);
            cargar_botones = true;
            sec.primera_vez = true;
            sec.primer_boton = true;
            estado = controlEventosPantallaJuego(&evento,&pantalla_jugador,estado,&jugador);
            mostrarPantallaJuego(&sdl,&pantalla_jugador);
            break;

        case OPCIONES:
            estado = eventosOpciones(&evento,&opciones,estado,&configuracion);
            mostrarPantallaOpciones(&sdl,&opciones);
            break;

        case ESTADISTICA:
            estado = controlEventosEstadistica(&evento,&pantalla_estadistica,estado);
            mostrarPantallaEstadistica(&sdl,&pantalla_estadistica);
            break;

        case SCHONBERG:
            ///pausarMusica(&sdl);
            estado = controlEventosSimon(&evento,boton_simon,configuracion.cant_botones,estado,botones_aux_simon,2,sonidos,&sec,deltaTime,&jugador);

            if(cargar_botones)
            {
                cargarBotonSimon(boton_simon, colores, colores_luz, configuracion.cant_botones, vector_valores_simon);
                cargar_botones = false;
                modo = true;
            }
            dibujarPantallaJuego(&sdl,fondo,boton_simon,configuracion.cant_botones,botones_aux_simon,2);
            if(sec.primera_vez)
            {
                jugador.Score = 0;
                int inicio = inicializarSecuencia(&sec,configuracion.cant_botones);
                if(inicio == MEM_ERROR)
                {
                    estado = SALIR;
                }
            }

            reproducirSecuencia(&sdl,sonidos,boton_simon,configuracion.cant_botones,fondo,botones_aux_simon,2,deltaTime,&sec,configuracion.duracion_inicial,-1);

            if(!sec.reproduciendo)
            {
                bool resultado = validarJugador(&jugador,&sec);

                if(resultado)
                {
                    jugador.Score += 10;
                    if(sec.indice >= sec.longitud)
                    {
                        jugador.Score += 10 * (sec.indice - 1);
                        printf("RONDA COMPLETA\n");
                        int agregar = agregarElemSecuencia(&sec,configuracion.cant_botones);
                        if(agregar == MEM_ERROR)
                        {
                            estado = SALIR;
                        }
                        sec.indice = 0;
                        sec.reproduciendo = true;
                    }
                }
                else if(jugador.valorBoton != -1)
                {
                    printf("PERDISTE\nPUNTUACION FINAL = %d\n", jugador.Score);

                    Mix_PlayChannel(1, sonidos[3], 0);

                    insertarEnTop(pantalla_estadistica.jugador, &pantalla_estadistica.ce_jugadores, jugador);

                    guardarTopEnArchivo(pantalla_estadistica.jugador, pantalla_estadistica.ce_jugadores, "top_schonberg.dat");

                    estado = PERDISTE;
                }

                jugador.valorBoton = -1; //restauro el valor despues de usar
            }
            break;

        case MOZART:
            ///pausarMusica(&sdl);
            if(sec.primera_vez)
            {
                size_t ce_mozart = contarElemSecuencia("secuencia.txt");
                inicializarSecuenciaMozart(&sec,ce_mozart);
                if((copiarSecuenciaMozart(&sec,"secuencia.txt",ce_mozart) == FORMATO_ERROR))
                {
                    printf("\nSecuencia con formato invalido.\n");
                    estado = JUGAR;
                }

                jugador.Score = 0;
                modo = false;

            }

            size_t cant_botones = buscarMaximo(&sec) + 1;

            estado = controlEventosSimon(&evento,boton_simon,cant_botones,estado,botones_aux_simon,2,sonidos,&sec,deltaTime,&jugador);

            if(cargar_botones)
            {
                cargarBotonSimon(boton_simon, colores, colores_luz,cant_botones, vector_valores_simon);
                cargar_botones = false;
            }
            dibujarPantallaJuego(&sdl,fondo,boton_simon,cant_botones,botones_aux_simon,2);

            reproducirSecuencia(&sdl,sonidos,boton_simon,cant_botones,fondo,botones_aux_simon,2,deltaTime,&sec,configuracion.duracion_inicial,sec.mozart_actual);

            if(!sec.reproduciendo)
            {
                bool resultado = validarJugador(&jugador,&sec);

                if(resultado)
                {
                    jugador.Score += 10;

                    if(sec.indice == sec.mozart_actual)
                    {
                        printf("RONDA COMPLETA\n");
                        jugador.Score += 10 * (sec.indice - 1);
                        sec.mozart_actual++;

                        if(sec.mozart_actual <= sec.longitud)
                        {
                            sec.indice = 0;
                            sec.reproduciendo = true;
                        }
                        else
                        {
                            insertarEnTop(pantalla_estadistica.jugador_mo, &pantalla_estadistica.ce_jugadores_mo, jugador);
                            guardarTopEnArchivo(pantalla_estadistica.jugador_mo, pantalla_estadistica.ce_jugadores_mo, "top_mozart.dat");
                            sec.mozart_actual = 1;
                            sec.indice = 0;
                            sec.reproduciendo = true;
                            win = true;
                            estado = GANASTE;
                        }
                    }
                }
                else if(jugador.valorBoton != -1)
                {
                    printf("PERDISTE");
                    Mix_PlayChannel(1, sonidos[3], 0);

                    insertarEnTop(pantalla_estadistica.jugador_mo, &pantalla_estadistica.ce_jugadores_mo, jugador);

                    guardarTopEnArchivo(pantalla_estadistica.jugador_mo, pantalla_estadistica.ce_jugadores_mo, "top_mozart.dat");

                    estado = PERDISTE;
                    sec.mozart_actual = 1;
                }
                jugador.valorBoton = -1; //restauro el valor despues de usar
            }
            break;

        case PERDISTE:
            cargar_botones = true;
            SDL_Color color_perdiste = (SDL_Color){255,0,0,3};
            pantalla_juego(&sdl,&jugador,win,color_perdiste,confeti);
            estado = controlEventosPantalla_juego(&evento,estado,modo);
            break;

        case GANASTE:
            cargar_botones = true;
            SDL_Color color_ganar = (SDL_Color){0,255,0,2};
            pantalla_juego(&sdl,&jugador,win,color_ganar,confeti);
            estado = controlEventosPantalla_juego(&evento,estado,modo);
            break;


        case SALIR:
            sec.primera_vez = false; // para que no libere memoria sin inicializar cuando salimos
            corriendo = false;
            break;
        }

        SDL_RenderPresent(sdl.renderer);
        SDL_Delay(16);
    }

    reiniciarJuego(&sec);
    limpiarSDL(&sdl);

    return 0;
}
