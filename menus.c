#include "menus.h"
#include "graficos.h"


bool _verificarMouseBoton(SDL_Rect boton, int mouse_x, int mouse_y)
{
    if(mouse_x >= boton.x && mouse_x <= boton.x + boton.w && mouse_y >= boton.y && mouse_y <= boton.y + boton.h)
        return true;
    return false;
}

void mostrarPantalla(tSistemaSDL *sdl, SDL_Color color, tBoton *boton, size_t ce_normal, tBoton_fondo *boton_fondo, size_t ce_fondo, unsigned int estado)
{
    SDL_SetRenderDrawBlendMode(sdl->renderer, SDL_BLENDMODE_BLEND);
    colorPantalla(sdl, color);
    dibujarFondo(sdl, boton_fondo, ce_fondo);
    dibujarTitulo(sdl);
    SDL_SetRenderDrawColor(sdl->renderer,60, 60, 60, 180);
    SDL_RenderFillRect(sdl->renderer,&(SDL_Rect){220,220,300,400});
    dibujar(sdl, boton, ce_normal);
    SDL_SetRenderDrawBlendMode(sdl->renderer, SDL_BLENDMODE_NONE);

}


void cargarDatosBotones(tBoton *boton, size_t ce,SDL_Color *colores,int vector_valores[], char* txt[])
{
    SDL_Color *pcolor = colores;
    int* pvec = vector_valores;
    char** ptxt = txt;
    unsigned int padding = 0;

    for(tBoton* i = boton; i < boton + ce; i++)
    {
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

void inicializarKonami(tKonami *codigo)
{
    codigo->indice = 0;
    codigo->longitud = 10;
}

bool verificarCodigoKonami(unsigned int valor_boton, tKonami *codigo)
{
    const unsigned int sec_konami[] = {1,1,6,6,3,4,3,4,0,7};

    if(valor_boton == sec_konami[codigo->indice])
    {
        codigo->indice++;
        if(codigo->indice == codigo->longitud)
        {
            codigo->indice = 0;
            printf("\nCODIGO KONAMI ACTIVADO\n");
            return true;
        }
    }
    else
    {
        codigo->indice = 0;
    }

    return false;
}


unsigned int controlEventos(SDL_Event *evento,tBoton* botones,size_t ce,tBoton_fondo *boton_fondo,unsigned int estado_actual, tKonami *codigo)
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

            for(tBoton_fondo* i = boton_fondo; i < boton_fondo + 8; i++)
            {
                i->hover = _verificarMouseBoton(i->rectangulo,evento->button.x, evento->button.y);
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

                for(tBoton_fondo*i = boton_fondo; i< boton_fondo + 8; i++)
                {
                    if(_verificarMouseBoton(i->rectangulo,evento->button.x,evento->button.y))
                    {
                        i->apretado = true;

                        if(verificarCodigoKonami(i->valor_boton,codigo))
                        {
                            printf("\nCHEAT ACTIVADO\n");
                        }

                        break;

                    }
                }

            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(evento->button.button == SDL_BUTTON_LEFT)
            {
                for(tBoton_fondo* i = boton_fondo; i < boton_fondo + 8; i++)
                {
                    i->apretado = false;
                }
            }
            break;


        case SDL_QUIT:
            bandera = SALIR;
            printf("\nEstado actual: Menu Saliendo\n");
            break;
        }
    }
    return bandera;
}


///IMPORTANTE
//Habria que modificar la funcion para que pueda recibir distintos tiutlos
//Y de esa manera poder evitar tener que identificar si se trata de un boton u otro
//Y mejorar la logica obviamente
//Pensar quizas una structura pantalla (una idea)
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


void dibujarFondo(tSistemaSDL *sdl, tBoton_fondo* boton_fondo, size_t ce_fondo)
{
    SDL_Color color_fondo;

    for(tBoton_fondo* i = boton_fondo; i < boton_fondo + ce_fondo; i++)
    {
        if(i->apretado == true)
        {
            color_fondo = i->color_apretado;
        }
        else if(i->hover == true)
        {
            color_fondo = i->color_hover;
        }
        else
        {
            color_fondo = i->color;
        }

        SDL_SetRenderDrawColor(sdl->renderer, color_fondo.r,color_fondo.g,color_fondo.b,color_fondo.a);
        SDL_RenderFillRect(sdl->renderer, &i->rectangulo);
        SDL_SetRenderDrawColor(sdl->renderer,0,0,0,255);
        SDL_RenderDrawRect(sdl->renderer,&i->rectangulo);
    }
}

void inicializarBoton_fondo(tBoton_fondo *boton_fondo, SDL_Color* colores,SDL_Color* color_hoverr, SDL_Color* color_apretadoo,int *v_valor)
{
    int tam_cuadrado = (ANCHO  - (ESPACIO * (CUADRADOS_POR_LADO - 1))) / CUADRADOS_POR_LADO;

    int j = 0;
    for(tBoton_fondo*i = boton_fondo; i < boton_fondo + 8; i++)
    {

        i->color = *colores;
        i->color_apretado = *color_apretadoo;
        i->color_hover = *color_hoverr;
        i->hover = false;
        i->apretado = false;
        i->valor_boton = v_valor[j];

        int indice = i - boton_fondo;

        int fila, columna;

        if(indice < 3)
        {
            fila = 0;
            columna = indice;
        }
        else if( indice < 5)
        {
            fila = 1;
            columna = (indice == 3) ? 0:2;
        }
        else
        {
            fila = 2;
            columna = indice - 5;
        }

        i->rectangulo = (SDL_Rect){columna*(tam_cuadrado + ESPACIO),fila*(tam_cuadrado + ESPACIO),tam_cuadrado,tam_cuadrado};

        colores++;
        color_apretadoo++;
        color_hoverr++;
        j++;
    }
}



