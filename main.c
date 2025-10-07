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


int main(int argc,char* argv[])
{
    //Variables para que funcione el programa
    bool corriendo = true;
    unsigned int estado = MENU;
    char nombreVentana[] = "TP Virus Simon";

    tSistemaSDL sdl;

    if(!inicializarSDL(&sdl,nombreVentana,ANCHO,LARGO))
    {
        printf("Error, no se pudo inicializar SDL\n");
        return 1;
    }

    reproducirMusica(&sdl);

    // Generacion de tonos
    Mix_Chunk* sonidos[2];
    crearArrayTonos(sonidos);

    //Vector colores
    SDL_Color colores[5] = {{255,0,0,255},{0,255,0,255},{0,0,255,255},{255,0,255,255},{255,0,255,255}};
    SDL_Color colores_luz[3] = {{255,255,255,255},{255,255,255,255},{255,255,255,255}};

    //vector valores para los botones de la pantalla
    int vector_valores_menu[] = {JUGAR, OPCIONES, ESTADISTICA, SALIR};
    int vector_valores_opciones[] = {OPCIONES_BOTONES, MENU};
    int vector_valores_estadistica[] = {MENU};
    int vector_valores_jugar[] = {SCHONBERG,MOZART,MENU};
    int vector_valores_simon[] = {BOTON_1,BOTON_2,BOTON_3};

    ///AUX
    int vector_valores_aux_simon[] = {JUGAR,SALIR};

    //Vector de textos
    char* texto_menu[] = {"jugar","opciones","estadistica","salir"};
    char* texto_opciones[] = {"Modificar","Volver"};
    char* texto_estadistica[] = {"Volver"};
    char* texto_jugar[] = {"Modo Schonberg", "Modo Mozart", "Volver"};

    ///AUX
    char* texto_aux_simon[] = {"Volver","Salir"};

    SDL_Event evento;

    //El bicho
    tSistemaCrab bicho_crab;
    inicializartSistemaCrab(&bicho_crab);

    //Cargamos datos a los botones
    tBoton botones_menu[CANTIDAD_BOTON_MENU];
    cargarDatosBotones(botones_menu,CANTIDAD_BOTON_MENU,colores, vector_valores_menu,texto_menu);

    tBoton botones_opciones[CANTIDAD_BOTON_OPCIONES];
    cargarDatosBotones(botones_opciones, CANTIDAD_BOTON_OPCIONES, colores,vector_valores_opciones,texto_opciones);

    tBoton botones_jugar[CANTIDAD_BOTON_JUGAR];
    cargarDatosBotones(botones_jugar,CANTIDAD_BOTON_JUGAR,colores,vector_valores_jugar,texto_jugar);

    tBoton botones_estadistica[CANTIDAD_BOTON_ESTADISTICA];
    cargarDatosBotones(botones_estadistica,CANTIDAD_BOTON_ESTADISTICA,colores,vector_valores_estadistica,texto_estadistica);

    ///AUX
    tBoton botones_aux_simon[2];
    cargarDatosBotones(botones_aux_simon,2,colores,vector_valores_aux_simon,texto_aux_simon);
    botones_aux_simon[0].rectangulo.x = 10;
    botones_aux_simon[0].rectangulo.y = 10;
    botones_aux_simon[1].rectangulo.x = 500;
    botones_aux_simon[1].rectangulo.y = 10;

    SDL_Color fondo = (SDL_Color){115,115,115,128};

    //Cargamos los datos al boton_simon
    tBotonSimon boton_simon[3];

    cargarBotonSimon(boton_simon, colores, colores_luz, 3,vector_valores_simon);

    while(corriendo)
    {
        switch(estado)
        {
        case MENU:
            estado = controlEventos(&evento,botones_menu,CANTIDAD_BOTON_MENU,estado);
            mostrarPantalla(&sdl,(SDL_Color){0,0,0,255},botones_menu,CANTIDAD_BOTON_MENU,&bicho_crab,estado);
            break;

        case JUGAR:
            reanudarMusica(&sdl);
            estado = controlEventos(&evento,botones_jugar,CANTIDAD_BOTON_JUGAR,estado);
            mostrarPantalla(&sdl,(SDL_Color){155,0,0,255},botones_jugar,CANTIDAD_BOTON_JUGAR,&bicho_crab,estado);
            break;

        case OPCIONES:
            estado = controlEventos(&evento,botones_opciones,CANTIDAD_BOTON_OPCIONES,estado);
            mostrarPantalla(&sdl,(SDL_Color){0,155,0,255},botones_opciones,CANTIDAD_BOTON_OPCIONES,&bicho_crab,estado);
            break;

        case ESTADISTICA:
            estado = controlEventos(&evento,botones_estadistica,CANTIDAD_BOTON_ESTADISTICA,estado);
            mostrarPantalla(&sdl,(SDL_Color){0,0,155,255},botones_estadistica,CANTIDAD_BOTON_ESTADISTICA,&bicho_crab,estado);
            break;

        case SCHONBERG:
            pausarMusica(&sdl);
            estado = controlEventosSimon(&evento,boton_simon,3,estado,botones_aux_simon,2);
            dibujarPantallaJuego(&sdl,fondo,boton_simon,3,botones_aux_simon,2);
            break;
        case SALIR:
            corriendo = false;
            break;
        }
        SDL_RenderPresent(sdl.renderer);
        SDL_Delay(16);
    }

    limpiarSDL(&sdl);

    return 0;
}
