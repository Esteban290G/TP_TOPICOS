#include "menus.h"
#include "graficos.h"


bool _verificarMouseBoton(SDL_Rect boton, int mouse_x, int mouse_y)
{
    if(mouse_x >= boton.x && mouse_x <= boton.x + boton.w && mouse_y >= boton.y && mouse_y <= boton.y + boton.h)
        return true;
    return false;
}

void cargarDatosBotones(tBoton *boton, size_t ce,SDL_Color *colores,int vector_valores[], char* txt[])
{
    SDL_Color *pcolor = colores;
    int* pvec = vector_valores;
    char** ptxt = txt;
    unsigned int padding = 0;

    for(tBoton* i = boton; i < boton + ce; i++)
    {
        i->color = *pcolor;
        i->rectangulo = (SDL_Rect){POSICION_X,POSICION_Y + padding, LARGO_RECT, ANCHO_RECT};
        i->valor_boton = *pvec;
        strcpy(i->texto_boton,*ptxt);
        i->hover = false;
        i->color_texto_normal = (SDL_Color){255,255,255,255};
        i->color_texto_hover = (SDL_Color){255,255,0,255};
        ptxt++;
        pcolor++;
        padding += PADDING;
        pvec++;
    }
}

void mostrarPantalla(tSistemaSDL *sdl, SDL_Color color, tBoton *boton, size_t ce, tSistemaCrab *bicho, unsigned int estado)
{
    actualizarCrab(bicho);
    colorPantalla(sdl, color);

    if(estado == MENU)
    {
        dibujarCrab(sdl, bicho);
        dibujarTitulo(sdl);
    }
    dibujar(sdl, boton, ce);
}



unsigned int controlEventos(SDL_Event *evento,tBoton* botones,size_t ce,unsigned int estado_actual)
{
    unsigned int bandera = estado_actual;
    while(SDL_PollEvent(evento))
    {
        switch(evento->type)
        {
        case SDL_MOUSEMOTION:
            for(tBoton* i = botones; i< botones + ce; i++)
            {
                i->hover = _verificarMouseBoton(i->rectangulo,evento->motion.x, evento->motion.y);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(evento->button.button == SDL_BUTTON_LEFT)
            {
                printf("Hiciste clic izquierdo en %d - %d\n",evento->button.x,evento->button.y);
                printf("\n");

                for(tBoton*i = botones; i < botones + ce; i++)
                {
                    if(_verificarMouseBoton(i->rectangulo,evento->button.x,evento->button.y))
                    {
                        printf("\nHiciste clic al boton numero %d\n",i->valor_boton);
                        bandera = i->valor_boton;
                    }
                }

            }
            break;
        case SDL_QUIT:
            bandera = SALIR;
            printf("\nEstado actual: Menu Saliendo\n");
            break;
        }
    }

    ///AUX
    if(bandera == OPCIONES_BOTONES || bandera == MOZART )
    {
        bandera = estado_actual;
    }
    return bandera;
}

void dibujarTitulo(tSistemaSDL* sdl)
{
    int x,y;

    Uint32 tiempo_actual = SDL_GetTicks();

    SDL_Color color;

    float velocidad = 250.0f;
    float factor = (sin(tiempo_actual/velocidad) + 1) / 2;
    color = (SDL_Color){255*factor, 255*(1-factor), 128 + 127*sin(tiempo_actual/600.0f), 255};
    TTF_Font* fuente_titulo = sdl->fuente_titulo;

    const char* texto = "SIMON: OUTBREAK";

    SDL_Surface* superficie = TTF_RenderText_Blended(fuente_titulo,texto,color);
    SDL_Texture* textura = SDL_CreateTextureFromSurface(sdl->renderer,superficie);

    x = (ANCHO - superficie->w) / 2 + (int)(20 * sin(tiempo_actual/300.0f));
    y = 60 + (int)(10 * fabs(sin(tiempo_actual/200.0f)));

    SDL_Rect rectangulo = {x,y,superficie->w,superficie->h};
    SDL_RenderCopy(sdl->renderer,textura,NULL,&rectangulo);

    SDL_DestroyTexture(textura);
    SDL_FreeSurface(superficie);
}




