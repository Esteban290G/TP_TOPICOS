#include "menus.h"
#include "graficos.h"


bool verificarClicBoton(SDL_Rect boton, int clic_x, int clic_y)
{
    if(clic_x >= boton.x && clic_x <= boton.x + boton.w && clic_y >= boton.y && clic_y <= boton.y + boton.h)
        return true;
    return false;
}

void menuPantalla(SDL_Renderer *renderer, SDL_Color color, botonMenu *boton,size_t ce)
{
    colorPantalla(renderer, color);

    boton[0].color = (SDL_Color){0,255,0,255};
    boton[1].color = (SDL_Color){0,0,255,255};
    boton[2].color = (SDL_Color){0,255,255,255};
    boton[3].color = (SDL_Color){255,0,0,255};

    boton[0].rectangulo = (SDL_Rect){275,300,LARGO_RECT,ANCHO_RECT};
    boton[1].rectangulo = (SDL_Rect){275,380,LARGO_RECT,ANCHO_RECT};
    boton[2].rectangulo = (SDL_Rect){275,460,LARGO_RECT,ANCHO_RECT};
    boton[3].rectangulo = (SDL_Rect){275,620,LARGO_RECT,ANCHO_RECT};

    boton[0].valor_boton = JUGAR;
    boton[1].valor_boton = OPCIONES;
    boton[2].valor_boton = ESTADISTICA;
    boton[3].valor_boton = SALIR;

    dibujar(renderer,boton,ce);

}

unsigned int controlEventos(SDL_Event *evento,const botonMenu* botones,size_t ce)
{

    unsigned int bandera;
    while(SDL_PollEvent(evento))
    {
        switch(evento->type)
        {
        case SDL_MOUSEBUTTONDOWN:
            if(evento->button.button == SDL_BUTTON_LEFT)
            {
                printf("Hiciste clic izquierdo en %d - %d\n",evento->button.x,evento->button.y);
                printf("\n");

                for(const botonMenu*i = botones; i < botones + ce; i++)
                {
                    if(verificarClicBoton(i->rectangulo,evento->button.x,evento->button.y))
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
    return bandera;
}

void menuJugar(SDL_Renderer *renderer, SDL_Color color, botonMenu* boton, size_t ce)
{
    colorPantalla(renderer, color);

    boton[0].color = (SDL_Color){25,255,0,255};
    boton[1].color = (SDL_Color){255,200,130,255};
    boton[2].color = (SDL_Color){200,20,255,255};
    boton[3].color = (SDL_Color){255,0,0,255};

    boton[0].rectangulo = (SDL_Rect){275,300,LARGO_RECT,ANCHO_RECT};
    boton[1].rectangulo = (SDL_Rect){275,380,LARGO_RECT,ANCHO_RECT};
    boton[2].rectangulo = (SDL_Rect){275,460,LARGO_RECT,ANCHO_RECT};
    boton[3].rectangulo = (SDL_Rect){275,540,LARGO_RECT,ANCHO_RECT};

    boton[0].valor_boton = JUGAR_FACIL;
    boton[1].valor_boton = JUGAR_NORMAL;
    boton[2].valor_boton = JUGAR_DIFICIL;
    boton[3].valor_boton = MENU;

    dibujar(renderer,boton,ce);
}

void menuOpciones(SDL_Renderer *renderer, SDL_Color color, botonMenu* boton, size_t ce)
{
    colorPantalla(renderer, color);

    boton[0].color = (SDL_Color){25,25,0,255};
    boton[1].color = (SDL_Color){255,0,0,255};


    boton[0].rectangulo = (SDL_Rect){275,300,LARGO_RECT,ANCHO_RECT};
    boton[1].rectangulo = (SDL_Rect){275,380,LARGO_RECT,ANCHO_RECT};

    boton[0].valor_boton = OPCIONES_BOTONES;
    boton[1].valor_boton = MENU;


    dibujar(renderer,boton,ce);
}

void menuEstadistica(SDL_Renderer *renderer, SDL_Color color, botonMenu* boton, size_t ce)
{
    colorPantalla(renderer, color);

    boton[0].color = (SDL_Color){255,0,0,255};

    boton[0].rectangulo = (SDL_Rect){275,300,LARGO_RECT,ANCHO_RECT};

    boton[0].valor_boton = MENU;


    dibujar(renderer,boton,ce);
}
