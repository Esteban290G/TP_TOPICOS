#include "juego.h"
#include "menus.h"
#include "graficos.h"

int generarAleatorio(int num_min, int num_max)
{
    int valor = rand() % (num_max - num_min + 1) + num_min;

    return valor;
}

void dibujarBoton(tSistemaSDL *sdl, tBotonSimon *boton,size_t ce)
{
    for(tBotonSimon* i = boton; i < boton + ce; i++)
    {
        SDL_SetRenderDrawColor(sdl->renderer, i->color_base.r, i->color_base.g, i->color_base.b, i->color_base.a);
        SDL_RenderFillRect(sdl->renderer, &i->rectangulo);
    }
}

void dibujarPantallaJuego(tSistemaSDL *sdl, SDL_Color color, tBotonSimon* boton_simon, size_t ce_simon, tBoton*boton_normal, size_t ce_normal)
{
    colorPantalla(sdl, color);
    dibujar(sdl,boton_normal,ce_normal);
    dibujarBoton(sdl, boton_simon,ce_simon);
}

void cargarBotonSimon(tBotonSimon* boton_simon, SDL_Color *color_1, SDL_Color *color_2, size_t ce, int* v_valor)
{
    SDL_Color *pcolor_base = color_1;
    SDL_Color *pcolor_sonando = color_2;
    int *p_valor = v_valor;
    unsigned int padding = 0;

    for (tBotonSimon *i = boton_simon; i < boton_simon + ce; i++)
    {
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

unsigned int controlEventosSimon(SDL_Event *evento, tBotonSimon *boton_simon,size_t ce_simon,unsigned int estado_actual, tBoton *boton_normal, size_t ce_normal)
{
    unsigned int bandera = estado_actual;
    while(SDL_PollEvent(evento))
    {
        switch(evento->type)
        {
        case SDL_MOUSEBUTTONDOWN:
            if(evento->button.button == SDL_BUTTON_LEFT)
            {
                printf("Hiciste clic izquierdo en %d - %d\n",evento->button.x,evento->button.y);
                printf("\n");

                for(tBotonSimon*i = boton_simon; i < boton_simon + ce_simon; i++)
                {
                    if(_verificarMouseBoton(i->rectangulo,evento->button.x,evento->button.y))
                    {
                        printf("\nHiciste clic al boton numero %d\n",i->valor_boton);
                        bandera = i->valor_boton;
                    }
                }
                for(tBoton* i = boton_normal; i < boton_normal + ce_normal; i++)
                {
                    if(_verificarMouseBoton(i->rectangulo,evento->button.x,evento->button.y))
                    {
                        printf("\nHiciste clic al boton numero %d\n",i->valor_boton);
                        bandera = i->valor_boton;
                    }
                }

            }
            break;

        case SDL_MOUSEMOTION:
            for(tBoton* i = boton_normal; i< boton_normal + ce_normal; i++)
            {
                i->hover = _verificarMouseBoton(i->rectangulo,evento->motion.x, evento->motion.y);
            }
            break;

        case SDL_QUIT:
            bandera = SALIR;
            printf("\nEstado actual: Menu Saliendo\n");
            break;
        }
    }

    ///AUX
    if(bandera == BOTON_1 || bandera == BOTON_2 || bandera == BOTON_3)
    {
        bandera = estado_actual;
    }
    return bandera;
}
