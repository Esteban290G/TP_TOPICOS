#include "juego.h"
#include "menus.h"
#include "graficos.h"

int generarAleatorio(int num_min, int num_max)
{
    int valor = rand() % (num_max - num_min + 1) + num_min;

    return valor;
}

void dibujarBoton(tSistemaSDL *sdl, tBotonSimon *boton, size_t ce)
{
    for (tBotonSimon *i = boton; i < boton + ce; i++)
    {
        SDL_SetRenderDrawColor(sdl->renderer, i->color_base.r, i->color_base.g, i->color_base.b, i->color_base.a);
        SDL_RenderFillRect(sdl->renderer, &i->rectangulo);
    }
}

void dibujarPantallaJuego(tSistemaSDL *sdl, SDL_Color color, tBotonSimon *boton_simon, size_t ce_simon, tBoton *boton_normal, size_t ce_normal)
{
    colorPantalla(sdl, color);
    dibujar(sdl, boton_normal, ce_normal);
    dibujarBoton(sdl, boton_simon, ce_simon);
}

void cargarBotonSimon(tBotonSimon *boton_simon, SDL_Color *color_1, SDL_Color *color_2, size_t ce, int *v_valor)
{
    SDL_Color *pcolor_base = color_1;
    SDL_Color *pcolor_sonando = color_2;
    int *p_valor = v_valor;
    unsigned int padding = 0;

    for (tBotonSimon *i = boton_simon; i < boton_simon + ce; i++)
    {
        i->tiempo_ultimo_sonido = 0; /// agregué recien
        i->color_aux = *pcolor_base;
        i->color_base = *pcolor_base;
        i->color_sonando = *pcolor_sonando;
        i->rectangulo = (SDL_Rect)
        {
            POSICION_X, POSICION_Y + padding, LARGO_SIMON, ANCHO_SIMON
        };
        i->valor_boton = *p_valor;
        i->sonando = false;

        pcolor_base++;
        pcolor_sonando++;
        p_valor++;
        padding += PADDING;
    }
}

int inicializarSecuencia(tSecuencia *secuencia, size_t cant_botones)
{
    secuencia->longitud = LONG_INICIAL;
    secuencia->tiempo_acumulado = 0;
    secuencia->indice = 0;
    secuencia->mostrando = true;
    secuencia->activo = false;
    secuencia->reproduciendo = true;
    secuencia->primera_vez = false;

    secuencia->vecSecuencia = malloc(sizeof(int)* secuencia->longitud);
    if(!secuencia->vecSecuencia)
    {
        printf("Error de memoria.\n");
        return MEM_ERROR;
    }

    secuencia->vecSecuencia[0] = generarAleatorio(N_MIN,cant_botones - 1);

    return MEM_OK;
}

int agregarElemSecuencia(tSecuencia *secuencia, size_t cant_botones)
{
    secuencia->longitud++;

    int *aux = realloc(secuencia->vecSecuencia,sizeof(int)* secuencia->longitud);
    if(!aux)
    {
        free(secuencia->vecSecuencia);
        printf("Error de memoria.\n");
        return MEM_ERROR;
    }

    secuencia->vecSecuencia = aux;

    secuencia->vecSecuencia[secuencia->longitud - 1] = generarAleatorio(N_MIN, cant_botones - 1);

    return MEM_OK;
}

void reproducirSecuencia(tSistemaSDL *sdl, Mix_Chunk* sonidos[], tBotonSimon *boton_simon, size_t ce_simon, SDL_Color color, tBoton *boton_normal, size_t ce_normal, float deltaTime, tSecuencia *secuencia)
{
    if(!secuencia->reproduciendo)
    {
        return;
    }

    int j;
    secuencia->tiempo_acumulado+= deltaTime;

    float tiempoPrendidoBase = 1400;
    float tiempoApagadoBase = 600;
    float tiempoEsperaInicio = 1500;

    float acelerador = 0.03f * (secuencia->longitud - 1);

    // un tope para que no se pase a negativo la resta
    if(acelerador > 1.0f)
    {
        acelerador = 0.9f;
    }

    float tiempoPrendido = tiempoPrendidoBase * (1.0f - acelerador);
    float tiempoApagado  = tiempoApagadoBase  * (1.0f - acelerador);

    if(secuencia->primer_boton && secuencia->tiempo_acumulado <= tiempoEsperaInicio)
    {
        return;
    }

    if(!secuencia->activo && secuencia->indice < secuencia->longitud)//primero para prender el boton si no hay ninguno prendido
    {
        j = secuencia->vecSecuencia[secuencia->indice];
        Mix_PlayChannel(1, sonidos[j], 0);
        boton_simon[j].color_base = boton_simon[j].color_sonando;
        secuencia->activo = true;
        secuencia->tiempo_acumulado = 0;
        secuencia->primer_boton = false;
    }
    else if (secuencia->activo && secuencia->mostrando && secuencia->tiempo_acumulado >= tiempoPrendido) //apagar el boton despues del tiempo prendido
    {
        j = secuencia->vecSecuencia[secuencia->indice];
        boton_simon[j].color_base = boton_simon[j].color_aux;
        secuencia->mostrando = false;
        secuencia->tiempo_acumulado = 0;
    }
    else if (secuencia->activo && !secuencia->mostrando && secuencia->tiempo_acumulado >= tiempoApagado) //cooldown para esperar un rato antes de pasar al siguiente
    {
        secuencia->indice++;
        secuencia->mostrando = true;
        secuencia->activo = false;
        secuencia->tiempo_acumulado = 0;

        if (secuencia->indice >= secuencia->longitud)
        {
            secuencia->indice = 0;
            secuencia->reproduciendo = false;
            secuencia->primer_boton = true;
        }
    }

    dibujarPantallaJuego(sdl, color, boton_simon, ce_simon, boton_normal, ce_normal);
}


unsigned int controlEventosSimon(SDL_Event *evento, tBotonSimon *boton_simon, size_t ce_simon, unsigned int estado_actual, tBoton *boton_normal, size_t ce_normal, Mix_Chunk *sonidos[],tSecuencia *secuencia, float deltaTime,tJugador *jugador)
{
    int i_sonido = 0;
    unsigned int estado = estado_actual;

    boton_simon->tiempo_ultimo_sonido += deltaTime; // para que haya un tiempo entre clics entre CUALQUIER boton
    float tiempo_espera = 500;

    while (SDL_PollEvent(evento))
    {
        switch (evento->type)
        {
        case SDL_MOUSEBUTTONDOWN:
            if (evento->button.button == SDL_BUTTON_LEFT)
            {
                printf("Hiciste clic izquierdo en %d - %d\n", evento->button.x, evento->button.y);
                printf("\n");

                for (tBotonSimon *i = boton_simon; i < boton_simon + ce_simon; i++)
                {
                    if (_verificarMouseBoton(i->rectangulo, evento->button.x, evento->button.y))
                    {
                        if (boton_simon->tiempo_ultimo_sonido >= tiempo_espera && !secuencia->reproduciendo && !i->sonando)
                        {
                            Mix_PlayChannel(1, sonidos[i_sonido], 0);
                            boton_simon->tiempo_ultimo_sonido = 0;

                            printf("\nHiciste clic al boton numero %d\n", i->valor_boton);
                            i->color_base = i->color_sonando;
                            i->sonando = true;

                            jugador->valorBoton = i->valor_boton;
                        }
                    }
                    i_sonido++;
                }
                for (tBoton *i = boton_normal; i < boton_normal + ce_normal; i++)
                {
                    if (_verificarMouseBoton(i->rectangulo, evento->button.x, evento->button.y))
                    {
                        printf("\nHiciste clic al boton numero %d\n", i->valor_boton);
                        estado = i->valor_boton;
                    }
                }
            }
            break;

        case SDL_MOUSEMOTION:
            for (tBoton *i = boton_normal; i < boton_normal + ce_normal; i++)
            {
                i->hover = _verificarMouseBoton(i->rectangulo, evento->motion.x, evento->motion.y);
            }
            break;

        case SDL_MOUSEBUTTONUP:

            if (evento->button.button == SDL_BUTTON_LEFT)
            {
                for (tBotonSimon *i = boton_simon; i < boton_simon + ce_simon; i++)
                {
                    i->color_base = i->color_aux;
                    i->sonando = false;
                }
            }
            break;

        case SDL_QUIT:
            estado = SALIR;
            printf("\nEstado actual: Menu Saliendo\n");
            break;
        }
    }

    return estado;
}

bool validarJugador(tJugador *jugador, tSecuencia *sec)
{
    bool correcto = false;

    if(jugador->valorBoton != -1)
    {
        if(sec->vecSecuencia[sec->indice] == jugador->valorBoton)
        {
            printf("CORRECTO\n");
            correcto = true;
            sec->indice++;
        }
        else
        {
            printf("INCORRECTO\n");
        }
    }

    return correcto;
}

void reiniciarJuego(tSecuencia *sec)
{
    free(sec->vecSecuencia);
    sec->vecSecuencia = NULL;
    sec->longitud = LONG_INICIAL;
    sec->indice = 0;
    sec->primera_vez = true;
    sec->primer_boton = true;
}
