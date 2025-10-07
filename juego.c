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
        i->color_aux = *pcolor_base;
        i->color_base = *pcolor_base;
        i->color_sonando = *pcolor_sonando;
        i->rectangulo = (SDL_Rect){POSICION_X, POSICION_Y + padding, LARGO_SIMON, ANCHO_SIMON};
        i->valor_boton = *p_valor;
        i->sonando = false;

        pcolor_base++;
        pcolor_sonando++;
        p_valor++;
        padding += PADDING;
    }
}

unsigned int controlEventosSimon(SDL_Event *evento, tBotonSimon *boton_simon, size_t ce_simon, unsigned int estado_actual, tBoton *boton_normal, size_t ce_normal, Mix_Chunk *sonidos[])
{
    static Uint32 ultimo_sonido_ms = 0;
    Uint32 ahora = SDL_GetTicks();
    /// AUX
    int i_sonido = 0;
    unsigned int bandera = estado_actual;
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
                        if (ahora - ultimo_sonido_ms > 500) //para que haya un tiempo entre clics al boton
                        {
                            Mix_PlayChannel(1, sonidos[i_sonido], 0);
                            ultimo_sonido_ms = ahora;

                            printf("\nHiciste clic al boton numero %d\n", i->valor_boton);
                            i->color_base = i->color_sonando;
                            i->sonando = true;

                            bandera = i->valor_boton;
                        }
                    }
                    i_sonido++;
                }
                for (tBoton *i = boton_normal; i < boton_normal + ce_normal; i++)
                {
                    if (_verificarMouseBoton(i->rectangulo, evento->button.x, evento->button.y))
                    {
                        printf("\nHiciste clic al boton numero %d\n", i->valor_boton);
                        bandera = i->valor_boton;
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
            bandera = SALIR;
            printf("\nEstado actual: Menu Saliendo\n");
            break;
        }
    }

    /// AUX
    if (bandera == BOTON_1 || bandera == BOTON_2 || bandera == BOTON_3)
    {
        bandera = estado_actual;
    }
    return bandera;
}

/*
///Bucle para la ventana y control de eventos
    while(!fin)
    {
        ///Me ayuda a poder dibujar en cada pasada de bucle
        int redibujar = 0;

        while(SDL_PollEvent(&evento))
        {
            switch(evento.type)
            {
            ///En caso que el evento sea un clic, "apagamos" la luz
            case SDL_MOUSEBUTTONDOWN:
                SDL_SetRenderDrawColor(render,160, 160, 160 , 255);
                SDL_RenderFillRect(render, &fondo);

                ///Verificamos que sea el clic izquierdo
                if(evento.button.button == SDL_BUTTON_LEFT)
                {
                    ///Utilizo un puntero de boton para poder manejar el boton apretado
                    Boton *botonclic = detectarBoton(boton, TOTAL_BOTONES,evento.button.x,evento.button.y);
                    ///Encaso que haya un boton apretado (!null) le asigno el atributo apretado
                    if(botonclic != NULL)
                    {
                        botonclic->apretado = APRETADO;;
                        redibujar = 1;
                    ///Aca para agregar el sonido le pedi ayuda a chatgpt, supuestamente ese if
                    ///es para evitar crasheos
                        if(sonido_boton)
                        {
                            Mix_PlayChannel(-1,sonido_boton,0);
                        }
                    }
                }
                break;
            ///Aca estamos controlando el evento del caso que se levante el clic, o sea ya no esta siendo precionado
            case SDL_MOUSEBUTTONUP:
                for(Boton* pos = boton;pos<boton + TOTAL_BOTONES;pos++)
                {
                    pos->apretado = NO_APRETADO;
                }
                ///Volvemos a prender la luz(?
                SDL_SetRenderDrawColor(render,230, 230, 230,255);
                redibujar = 1;
                break;
            case SDL_QUIT:
                fin = true;
                break;
            }
        }
    } */
