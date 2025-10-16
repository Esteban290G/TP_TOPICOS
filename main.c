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

int main(int argc, char *argv[])
{
    // Variables para iniciar el programa
    bool corriendo = true;
    unsigned int estado = MENU;
    char nombreVentana[] = "TP Virus Simon";

    // Variables para el tiempo
    Uint32 tiempoAnterior = SDL_GetTicks();
    float delta_time = 0;

    // Inicializar SDL
    tSistemaSDL sdl;

    if (!inicializarSDL(&sdl, nombreVentana, ANCHO, LARGO))
    {
        printf("\nError, no se pudo inicializar SDL\n");
        return 1;
    }

    reproducirMusica(&sdl);

    // Cargar y generar los sonidos
    Mix_Chunk *sonidos[8];
    Mix_Chunk *sonido_error = cargarSonido("snd/sonido_error.wav");

    Mix_Chunk *armas[8];
    cargarSonidosCounter(armas);

    Mix_Chunk *mine[8];
    cargarSonidosMinecraft(mine);

    //  Colores Rojo - Verde - Azul - Magenta - Amarillo - Cian - Naranja - Morado
    SDL_Color colores[] = {{191, 0, 0, 255}, {0, 191, 0, 255}, {0, 0, 191, 255}, {191, 0, 191, 255}, {191, 191, 0, 255}, {0, 191, 191, 255}, {191, 124, 0, 255}, {65, 26, 75, 255}};
    SDL_Color colores_luz[] = {{236, 83, 83, 255}, {83, 236, 83, 255}, {83, 83, 236, 255}, {236, 83, 236, 255}, {236, 236, 83, 255}, {83, 236, 236, 255}, {236, 182, 83, 255}, {104, 52, 117, 255}};
    SDL_Color fondo = (SDL_Color){115, 115, 115, 128};
    SDL_Color colores_apretado[] = {{100, 0, 0, 255}, {0, 100, 0, 255}, {0, 0, 100, 255}, {100, 0, 100, 255}, {120, 120, 0, 255}, {0, 100, 100, 255}, {120, 70, 0, 255}, {30, 10, 40, 255}};

    size_t cant_simon_menu = 8; // Para los botones de colores que aparecen en menu ppal

    // Estructuras para el juego
    SDL_Event evento;
    tPantallaJugador pantalla_jugador;
    tJugador jugador;
    tEstadistica pantalla_estadistica;
    tOpciones opciones;
    tSecuencia sec; // secuencia del simon
    tKonami codigo;
    tConfigJuego configuracion;
    tBotonSimon boton_simon[8];

    inicializarPantallaJugador(&pantalla_jugador);
    inicializarJugador(&pantalla_jugador, &jugador);
    inicializarPantallaEstadistica(&pantalla_estadistica);
    inicializarOpciones(&opciones);
    inicializarKonami(&codigo);

    // Cargar archivos para estadisticas
    const char* stats_schon = "top_schonberg.dat";
    const char* stats_mozart = "top_mozart.dat";
    char* archivo_sec = "secuencia.txt";
    crearArchivoSecuencia(archivo_sec);


    cargarTopDesdeArchivo(pantalla_estadistica.jugador, &pantalla_estadistica.ce_jugadores, stats_schon);
    cargarTopDesdeArchivo(pantalla_estadistica.jugador_mo, &pantalla_estadistica.ce_jugadores_mo, stats_mozart);

    /// Datos de botones y texto
    int vector_valores_menu[] = {JUGAR, OPCIONES, ESTADISTICA, SALIR};
    int vector_valores_simon[] = {BOTON_1, BOTON_2, BOTON_3, BOTON_4, BOTON_5, BOTON_6, BOTON_7, BOTON_8};
    int vector_valores_menu_simon[] = {JUGAR, SALIR};
    int vector_valores_desafio[] = {JUGAR, GUARDAR};

    char *texto_menu[] = {"Jugar", "Opciones", "Estadisticas", "Salir"};
    char *texto_simon[] = {"Volver", "Salir"};
    char *texto_desafio[] = {"Volver", "Guardar"};

    // Cargar datos a los botones
    tBoton botones_menu[CANTIDAD_BOTON_MENU];
    cargarDatosBotones(botones_menu, CANTIDAD_BOTON_MENU, colores, vector_valores_menu, texto_menu);

    // Botones de menu dentro del juego SE PUEDE HACER EN FUNCION CAPAZ
    tBoton botones_aux_simon[2];
    cargarDatosBotones(botones_aux_simon, 2, colores, vector_valores_menu_simon, texto_simon);
    botones_aux_simon[0].rectangulo.x = 10;
    botones_aux_simon[0].rectangulo.y = 10;
    botones_aux_simon[1].rectangulo.x = 500;
    botones_aux_simon[1].rectangulo.y = 10;

    tBoton botones_aux_desafio[2];
    cargarDatosBotones(botones_aux_desafio, 2, colores, vector_valores_desafio, texto_desafio);
    botones_aux_desafio[0].rectangulo.x = 10;
    botones_aux_desafio[0].rectangulo.y = 10;
    botones_aux_desafio[1].rectangulo.x = 500;
    botones_aux_desafio[1].rectangulo.y = 10;

    // Botones de fondo del menu principal
    tBoton_fondo boton_fondo[8];
    tFlecha flecha[4];

    inicializarBoton_fondo(boton_fondo, colores, colores_luz, colores_apretado, vector_valores_simon);
    inicializarFlecha(flecha);

    // variables necesarias para la logica
    bool modo = true;
    bool win = false;
    bool cargar_botones = true;
    bool cargar_sonidos = true;
    int cant_tonos = 0;
    configuracion.cant_botones = 3;
    configuracion.duracion_inicial = 2000;

    while (corriendo)
    {
        // variables para manejar tiempos
        Uint32 tiempoActual = SDL_GetTicks();
        delta_time = tiempoActual - tiempoAnterior;
        tiempoAnterior = tiempoActual;

        switch (estado)
        {
        case MENU:
            reanudarMusica(&sdl);
            pantalla_jugador.hay_secuencia = true;
            estado = controlEventos(&evento, botones_menu, CANTIDAD_BOTON_MENU, boton_fondo, estado, &codigo,flecha);

            if (codigo.cheat)
            {
                jugador.vidas = VIDAS_CHEAT;
            }

            mostrarPantalla(&sdl, (SDL_Color){0, 0, 0, 255}, botones_menu, CANTIDAD_BOTON_MENU, boton_fondo, cant_simon_menu, flecha,estado);
            break;

        case JUGAR:
            reanudarMusica(&sdl);
            cargar_botones = true;
            sec.primera_vez = true;
            sec.primer_boton = true;
            estado = controlEventosPantallaJuego(&evento, &pantalla_jugador, estado, &jugador);
            mostrarPantallaJuego(&sdl, &pantalla_jugador);
            break;

        case OPCIONES:
            estado = eventosOpciones(&evento, &opciones, estado, &configuracion);
            mostrarPantallaOpciones(&sdl, &opciones);

            if(opciones.config.es_counter)
            {
                for(int i = 0; i<8; i++)
                {
                    sonidos[i] = armas[i];
                }
                cargar_sonidos = false;
            }
            else if(opciones.config.es_minecraft)
            {
                for(int i = 0; i<8; i++)
                {
                    sonidos[i] = mine[i];
                }
                cargar_sonidos = false;
            }
            else
            {
                cargar_sonidos = true;
            }

            break;

        case ESTADISTICA:
            estado = controlEventosEstadistica(&evento, &pantalla_estadistica, estado);
            mostrarPantallaEstadistica(&sdl, &pantalla_estadistica);
            break;

        case SCHONBERG:
            pausarMusica(&sdl);
            estado = controlEventosSimon(&evento, boton_simon, configuracion.cant_botones, estado, botones_aux_simon, 2, sonidos, &sec, delta_time, &jugador);

            if(cargar_sonidos)
            {
                cant_tonos = crearArrayTonos(sonidos, configuracion.cant_botones);
                cargar_sonidos = false;
            }

            if (cargar_botones)
            {
                cargarBotonSimon(boton_simon, colores, colores_luz, configuracion.cant_botones, vector_valores_simon);
                cargar_botones = false;
                modo = true;
            }
            dibujarPantallaJuego(&sdl, fondo, boton_simon, configuracion.cant_botones, botones_aux_simon, 2);
            if (sec.primera_vez)
            {
                jugador.Score = 0;
                int inicio = inicializarSecuencia(&sec, configuracion.cant_botones);
                if (inicio == MEM_ERROR)
                {
                    estado = SALIR;
                }
            }

            reproducirSecuencia(&sdl, sonidos, boton_simon, configuracion.cant_botones, fondo, botones_aux_simon, 2, delta_time, &sec, configuracion.duracion_inicial, -1);

            if (!sec.reproduciendo)
            {
                bool resultado = validarJugador(&jugador, &sec);

                if (resultado)
                {
                    jugador.Score += 10;
                    if (sec.indice >= sec.longitud)
                    {
                        jugador.Score += 10 * (sec.indice - 1);
                        printf("RONDA COMPLETA\n");
                        int agregar = agregarElemSecuencia(&sec, configuracion.cant_botones);
                        if (agregar == MEM_ERROR)
                        {
                            estado = SALIR;
                        }
                        sec.indice = 0;
                        sec.reproduciendo = true;
                    }
                }
                else if ((jugador.valorBoton != -1 && !codigo.cheat) || (codigo.cheat && jugador.vidas == 0))
                {
                    printf("PERDISTE\nPUNTUACION FINAL = %d\n", jugador.Score);

                    Mix_PlayChannel(1, sonido_error, 0);

                    insertarEnTop(pantalla_estadistica.jugador, &pantalla_estadistica.ce_jugadores, jugador);

                    guardarTopEnArchivo(pantalla_estadistica.jugador, pantalla_estadistica.ce_jugadores, stats_schon);
                    estado = PERDISTE;
                }
                else if (jugador.valorBoton != -1 && codigo.cheat)
                {
                    jugador.vidas--;
                }

                jugador.valorBoton = -1; // restauro el valor despues de usar
            }
            break;

        case MOZART:
            pausarMusica(&sdl);
            if (sec.primera_vez)
            {
                size_t ce_mozart = contarElemSecuencia(archivo_sec);
                inicializarSecuenciaMozart(&sec, ce_mozart);
                if ((copiarSecuenciaMozart(&sec, archivo_sec, ce_mozart) == FORMATO_ERROR))
                {
                    pantalla_jugador.hay_secuencia = false;
                    printf("\nSecuencia con formato invalido.\n");
                    estado = JUGAR;
                }

                jugador.Score = 0;
                modo = false;
            }

            size_t cant_botones = 3;
            size_t cant_aux = buscarMaximo(&sec) + 1;
            if(cant_aux >= cant_botones)
            {
                cant_botones = cant_aux;
            }

            estado = controlEventosSimon(&evento, boton_simon, cant_botones, estado, botones_aux_simon, 2, sonidos, &sec, delta_time, &jugador);

            if(cargar_sonidos)
            {
                cant_tonos = crearArrayTonos(sonidos, configuracion.cant_botones);
                cargar_sonidos = false;
            }

            if (cargar_botones)
            {
                cargarBotonSimon(boton_simon, colores, colores_luz, cant_botones, vector_valores_simon);
                cargar_botones = false;
            }
            dibujarPantallaJuego(&sdl, fondo, boton_simon, cant_botones, botones_aux_simon, 2);

            reproducirSecuencia(&sdl, sonidos, boton_simon, cant_botones, fondo, botones_aux_simon, 2, delta_time, &sec, configuracion.duracion_inicial, sec.mozart_actual);

            if (!sec.reproduciendo)
            {
                bool resultado = validarJugador(&jugador, &sec);

                if (resultado)
                {
                    jugador.Score += 10;

                    if (sec.indice == sec.mozart_actual)
                    {
                        printf("RONDA COMPLETA\n");
                        jugador.Score += 10 * (sec.indice - 1);
                        sec.mozart_actual++;

                        if (sec.mozart_actual <= sec.longitud)
                        {
                            sec.indice = 0;
                            sec.reproduciendo = true;
                        }
                        else
                        {
                            insertarEnTop(pantalla_estadistica.jugador_mo, &pantalla_estadistica.ce_jugadores_mo, jugador);
                            guardarTopEnArchivo(pantalla_estadistica.jugador_mo, pantalla_estadistica.ce_jugadores_mo, stats_mozart);
                            sec.mozart_actual = 1;
                            sec.indice = 0;
                            sec.reproduciendo = true;
                            win = true;
                            estado = GANASTE;
                        }
                    }
                }
                else if ((jugador.valorBoton != -1 && !codigo.cheat) || (codigo.cheat && jugador.vidas == 0))
                {
                    printf("PERDISTE");
                    Mix_PlayChannel(1, sonido_error, 0);

                    insertarEnTop(pantalla_estadistica.jugador_mo, &pantalla_estadistica.ce_jugadores_mo, jugador);

                    guardarTopEnArchivo(pantalla_estadistica.jugador_mo, pantalla_estadistica.ce_jugadores_mo, stats_mozart);
                    estado = PERDISTE;
                    sec.mozart_actual = 1;
                }
                else if (jugador.valorBoton != -1 && codigo.cheat)
                {
                    jugador.vidas--;
                }
                jugador.valorBoton = -1; // restauro el valor despues de usar
            }
            break;

        case DESAFIO:
            pausarMusica(&sdl);
            if(sec.primera_vez)
            {
                inicializarSecuenciaDesafio(&sec);
                sec.primera_vez = false;
            }

            estado = controlEventosSimon(&evento, boton_simon, configuracion.cant_botones, estado, botones_aux_desafio, 2, sonidos, &sec, delta_time, &jugador);

            if(cargar_sonidos)
            {
                cant_tonos = crearArrayTonos(sonidos, configuracion.cant_botones);
            }

            if (cargar_botones)
            {
                cargarBotonSimon(boton_simon, colores, colores_luz, configuracion.cant_botones, vector_valores_simon);
                cargar_botones = false;
            }
            dibujarPantallaJuego(&sdl, fondo, boton_simon, configuracion.cant_botones, botones_aux_desafio, 2);

            if (estado == GUARDAR)
            {
                agregarSecuenciaArchivo(&sec,archivo_sec);
                estado = MENU;

            }
            else if (jugador.valorBoton >= BOTON_1 && jugador.valorBoton <= BOTON_8)
            {
                agregarDesafioSecuencia(&sec,jugador.valorBoton);
                jugador.valorBoton = -1;
            }

            break;

        case PERDISTE:
            codigo.cheat = false;
            cargar_botones = true;
            SDL_Color color_perdiste = (SDL_Color){255, 0, 0, 3};
            pantalla_juego(&sdl, &jugador, win, color_perdiste);
            estado = controlEventosPantalla_juego(&evento, estado, modo);
            break;

        case GANASTE:
            codigo.cheat = false;
            cargar_botones = true;
            SDL_Color color_ganar = (SDL_Color){0, 255, 0, 2};
            pantalla_juego(&sdl, &jugador, win, color_ganar);
            estado = controlEventosPantalla_juego(&evento, estado, modo);
            break;

        case SALIR:
            sec.primera_vez = false; // para que no libere memoria sin inicializar cuando salimos
            corriendo = false;
            break;
        }

        SDL_RenderPresent(sdl.renderer);
        SDL_Delay(16);
    }

    // Limpiar memoria
    cant_tonos = crearArrayTonos(sonidos, 8); // Para que no haya problemas con destruirArrayTonos
    destruirArrayTonos(sonidos,cant_tonos);
    destruirArraySonido(armas);
    destruirArraySonido(mine);
    Mix_FreeChunk(sonido_error);
    Mix_FreeChunk(codigo.sonido_ok);
    reiniciarJuego(&sec);
    limpiarSDL(&sdl);

    return 0;
}
