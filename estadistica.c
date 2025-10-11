#include "estadistica.h"

float _signo(int px, int py, SDL_Point p1, SDL_Point p2)
{
    return (px - p2.x) * (p1.y - p2.y) - (p1.x - p2.x) * (py -p2.y);
}


bool _verificarTriangulo(tBoton_triangular* boton_triangular, int mouse_x, int mouse_y)
{
    SDL_Point vertices[3];
    int ancho_triangulo = 12 *5;
    int alto_triangulo = 12 *5;

    if(boton_triangular->estado == MOZART)
    {
        vertices[0] = (SDL_Point){boton_triangular->posicion.x,
        boton_triangular->posicion.y};

        vertices[1] = (SDL_Point){boton_triangular->posicion.x, boton_triangular->posicion.y + alto_triangulo};

        vertices[2] = (SDL_Point){boton_triangular->posicion.x + ancho_triangulo, boton_triangular->posicion.y + alto_triangulo/2};
    }
    else
    {
        vertices[0] = (SDL_Point){boton_triangular->posicion.x + ancho_triangulo, boton_triangular->posicion.y};

        vertices[1] = (SDL_Point){boton_triangular->posicion.x + ancho_triangulo, boton_triangular->posicion.y + alto_triangulo};

        vertices[2] = (SDL_Point){boton_triangular->posicion.x, boton_triangular->posicion.y + alto_triangulo/2};
    }

    float d1,d2,d3;

    bool negativo, positivo;

    d1 = _signo(mouse_x, mouse_y, vertices[0], vertices[1]);
    d2 = _signo(mouse_x, mouse_y, vertices[1], vertices[2]);
    d3 = _signo(mouse_x, mouse_y, vertices[2], vertices[0]);


    negativo = (d1 < 0) || (d2 < 0) || (d3 < 0);
    positivo = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(negativo && positivo);
}


int _cantidadRegistros(FILE *pf)
{
    int bytes, cantidad;

    fseek(pf,0,SEEK_END);

    bytes = ftell(pf);
    cantidad = bytes/sizeof(tJugador);

    fclose(pf);

    return cantidad;
}

FILE* corroborarArchivo(char* texto)
{
    FILE *pf = fopen(texto,"rb");
    if(pf == NULL)
    {
        pf = fopen(texto,"wb");

        if(pf == NULL)
        {
            printf("Error\n");
            return NULL;
        }
    }
    return pf;
}

int agregarDatosVector(tJugador *jugador, char* texto)
{
    FILE *pf;
    tJugador *pj = jugador;
    size_t ce;


    pf = corroborarArchivo(texto);

    if(!pf)
    {
        printf("Archivo creado");
        fclose(pf);
        return ARCHIVO_CREADO;
    }

    if(!(ce = _cantidadRegistros(pf)))
    {
        printf("No hay datos cargados para almacenar en los vectores");
        fclose(pf);
        return ce;
    }

    fread(jugador,sizeof(tJugador),1,pf);
    while(!feof(pf))
    {
        *pj = *jugador;
        pj++;
        fread(jugador,sizeof(tJugador),1,pf);
    }
    fclose(pf);

    return ce;
}

void insertarVectorOrdenado(tJugador *jugador, size_t* ce, tJugador nuevo)
{
    tJugador *pfin = jugador + *ce - 1;
    tJugador *ppos = jugador;

    if(*ce == MAX_JUGADORES)
    {
        (*ce)--;
    }

    while(ppos <= pfin && ppos->Score > nuevo.Score)
    {
        ppos++;
    }
    while(pfin >= ppos)
    {
        *(pfin+1) = *pfin;
        pfin--;
    }
    *ppos = nuevo;
    (*ce)++;
}

bool agregarArchivo(tJugador* jugador, size_t ce, char* texto)
{
    FILE *pf;
    pf = corroborarArchivo(texto);

    if(!pf)
    {
        printf("No existe el archivo");
        fclose(pf);
        return false;
    }

    for(tJugador* i = jugador; i < jugador + ce; i++)
    {
        fwrite(i,sizeof(tJugador),1,pf);
    }

    fclose(pf);
    return true;
}

void inicializarPantallaEstadistica(tEstadistica* estadistica)
{
    estadistica->botones[0].valor_boton = MENU;
    estadistica->botones[0].color_texto_normal = (SDL_Color){255,255,255,255};
    estadistica->botones[0].color_texto_hover = (SDL_Color){255,255,0,255};
    estadistica->botones[0].hover = false;
    strcpy(estadistica->botones[0].texto_boton,"volver");
    estadistica->botones[0].rectangulo = (SDL_Rect)
    {
        30,
        680,
        LARGO_RECT,
        ANCHO_RECT
    };

    estadistica->rec_fondo = (SDL_Color){255,255,255,150};
    estadistica->rec_bordes = (SDL_Color){0,0,0,255};
    estadistica->ce_botones_normales = CANTIDAD_BOTON_ESTADISTICA;
    estadistica->fondo = (SDL_Color){0,0,0,255};
    estadistica->color_titulo = (SDL_Color){255,255,255,255};
    estadistica->color_texto = (SDL_Color){255,255,255,255};

    estadistica->botones_triangulares[0].color_relleno = (SDL_Color){255,255,255,255};
    estadistica->botones_triangulares[1].color_relleno = (SDL_Color){255,255,255,255};

    estadistica->botones_triangulares[0].color_bordes = (SDL_Color){255,255,0,255};
    estadistica->botones_triangulares[1].color_bordes = (SDL_Color){255,255,0,255};

    estadistica->botones_triangulares[0].posicion = (SDL_Point){500,580};
    estadistica->botones_triangulares[1].posicion = (SDL_Point){200,580};

    estadistica->botones_triangulares[0].estado = MOZART;
    estadistica->botones_triangulares[1].estado = SCHONBERG;




    estadistica->ce_jugadores = agregarDatosVector(estadistica->jugador,"estadistica.dat");
    estadistica->rec_estadistica = (SDL_Rect)
    {
        180,
        180,
        LARGO_RECT * 2,
        ANCHO_RECT * 7
    };
    estadistica->ce_jugadores_mo = agregarDatosVector(estadistica->jugador_mo,"estadisticamo.dat");

    strcpy(estadistica->titulo,"Estadistica");

    ///AUX
    int multi = 100;
    if(estadistica->ce_jugadores == 0)
    {
        for(tJugador* i = estadistica->jugador; i < estadistica->jugador + MAX_JUGADORES; i++)
        {
            strcpy(i->nombre,"lucas");
            i->Score = multi;

            multi += 100;
        }

        estadistica->ce_jugadores = MAX_JUGADORES;

        agregarArchivo(estadistica->jugador,estadistica->ce_jugadores,"estadistica.dat");
    }

    if(estadistica->ce_jugadores_mo == 0)
    {
        for(tJugador* i = estadistica->jugador_mo; i < estadistica->jugador_mo + MAX_JUGADORES; i++)
        {
            strcpy(i->nombre,"PEDROOO");
            i->Score = multi;

            multi += 100;
        }

        estadistica->ce_jugadores_mo = MAX_JUGADORES;

        agregarArchivo(estadistica->jugador,estadistica->ce_jugadores,"estadisticamo.dat");
    }


    estadistica->es_schon = true;


}

void mostrarTextoEst(tSistemaSDL* sdl, tEstadistica* estadistica, char* texto_est)
{
    int x, y;
    SDL_Color color;
    char *texto;

    texto = texto_est;
    color = estadistica->color_titulo;

    TTF_Font *fuente_titulo = sdl->fuente;

    SDL_Surface *superficie = TTF_RenderText_Blended(fuente_titulo, texto, color);
    SDL_Texture *textura = SDL_CreateTextureFromSurface(sdl->renderer, superficie);

    x = 310;
    y = 600;

    SDL_Rect rectangulo = {x, y, superficie->w, superficie->h};
    SDL_RenderCopy(sdl->renderer, textura, NULL, &rectangulo);

    SDL_DestroyTexture(textura);
    SDL_FreeSurface(superficie);
}

void mostrarPantallaEstadistica(tSistemaSDL*sdl ,tEstadistica* estadistica)
{
    if(estadistica->es_schon)
    {
        colorPantalla(sdl,estadistica->fondo);
        SDL_SetRenderDrawColor(sdl->renderer, estadistica->rec_fondo.r,estadistica->rec_fondo.g,estadistica->rec_fondo.b,estadistica->rec_fondo.a);
        SDL_RenderFillRect(sdl->renderer, &estadistica->rec_estadistica);

        SDL_SetRenderDrawColor(sdl->renderer, estadistica->rec_bordes.r,estadistica->rec_bordes.g,estadistica->rec_bordes.b,estadistica->rec_bordes.a);
        SDL_RenderDrawRect(sdl->renderer, &estadistica->rec_estadistica);

        dibujar(sdl,estadistica->botones, estadistica->ce_botones_normales);

        mostrarJugadores(sdl,estadistica);
        mostrarTituloEstadistica(sdl,estadistica);

        dibujarBotonesTriangulares(sdl,estadistica->botones_triangulares,2);

        mostrarTextoEst(sdl,estadistica,"Schonberg");
    }
    else
    {
        colorPantalla(sdl,estadistica->fondo);
        SDL_SetRenderDrawColor(sdl->renderer, estadistica->rec_fondo.r,estadistica->rec_fondo.g,estadistica->rec_fondo.b,estadistica->rec_fondo.a);
        SDL_RenderFillRect(sdl->renderer, &estadistica->rec_estadistica);

        SDL_SetRenderDrawColor(sdl->renderer, estadistica->rec_bordes.r,estadistica->rec_bordes.g,estadistica->rec_bordes.b,estadistica->rec_bordes.a);
        SDL_RenderDrawRect(sdl->renderer, &estadistica->rec_estadistica);

        dibujar(sdl,estadistica->botones, estadistica->ce_botones_normales);
        mostrarJugadores_mo(sdl,estadistica);
        mostrarTituloEstadistica(sdl,estadistica);

        dibujarBotonesTriangulares(sdl,estadistica->botones_triangulares,2);

        mostrarTextoEst(sdl,estadistica,"Mozart");
    }

}

void mostrarTituloEstadistica(tSistemaSDL* sdl, tEstadistica* estadistica)
{
    int x, y;
    SDL_Color color;
    char *texto;

    texto = estadistica->titulo;
    color = estadistica->color_titulo;

    TTF_Font *fuente_titulo = sdl->fuente_titulo;

    SDL_Surface *superficie = TTF_RenderText_Blended(fuente_titulo, texto, color);
    SDL_Texture *textura = SDL_CreateTextureFromSurface(sdl->renderer, superficie);

    x = (ANCHO - superficie->w) / 2;
    y = 60;

    SDL_Rect rectangulo = {x, y, superficie->w, superficie->h};
    SDL_RenderCopy(sdl->renderer, textura, NULL, &rectangulo);

    SDL_DestroyTexture(textura);
    SDL_FreeSurface(superficie);
}

unsigned int controlEventosEstadistica(SDL_Event* evento, tEstadistica* estadistica, unsigned int estado_actual)
{

   unsigned int bandera = estado_actual;
    while(SDL_PollEvent(evento))
    {
        switch(evento->type)
        {
        case SDL_MOUSEMOTION:
            for(tBoton* i = estadistica->botones; i< estadistica->botones + estadistica->ce_botones_normales; i++)
            {
                i->hover = _verificarMouseBoton(i->rectangulo,evento->motion.x, evento->motion.y);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(evento->button.button == SDL_BUTTON_LEFT)
            {
                printf("Hiciste clic izquierdo en %d - %d\n",evento->button.x,evento->button.y);
                printf("\n");

                for(tBoton*i = estadistica->botones; i < estadistica->botones + estadistica->ce_botones_normales; i++)
                {
                    if(_verificarMouseBoton(i->rectangulo,evento->button.x,evento->button.y))
                    {
                        printf("\nHiciste clic al boton numero %d\n",i->valor_boton);
                        bandera = i->valor_boton;

                    }
                }

                for(int i = 0; i < 2; i++)
                {
                    if(_verificarTriangulo(&estadistica->botones_triangulares[i],evento->button.x, evento->button.y))
                    {
                        printf("Hiciste click en el triangulo");
                        if(estadistica->botones_triangulares[i].estado == SCHONBERG)
                            bandera = EST_SCHO;
                        else if(estadistica->botones_triangulares[i].estado == MOZART)
                            bandera = EST_MO;
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
    if(bandera == EST_MO)
    {
        estadistica->es_schon = false;
        bandera = estado_actual;
    }
    if(bandera == EST_SCHO)
    {
        estadistica->es_schon = true;
        bandera = estado_actual;
    }
    return bandera;
}

void mostrarJugadores(tSistemaSDL* sdl, tEstadistica* estadistica)
{
    int x = estadistica->rec_estadistica.x + 10;
    int y = estadistica->rec_estadistica.y + 30;
    int separador = 40;

    char texto[200];

    for(tJugador *i = estadistica->jugador; i < estadistica->jugador + estadistica->ce_jugadores; i++)
    {
        sprintf(texto,"%s - %d pts",i->nombre,i->Score);

        SDL_Surface *superficie = TTF_RenderText_Blended(sdl->fuente,texto,estadistica->color_texto);
        SDL_Texture *textura = SDL_CreateTextureFromSurface(sdl->renderer,superficie);

        SDL_Rect rect = {x, y + separador, superficie->w, superficie->h};
        SDL_RenderCopy(sdl->renderer,textura,NULL,&rect);

        SDL_FreeSurface(superficie);
        SDL_DestroyTexture(textura);

        separador += 40;
    }
}

void mostrarJugadores_mo(tSistemaSDL* sdl, tEstadistica* estadistica)
{
    int x = estadistica->rec_estadistica.x + 10;
    int y = estadistica->rec_estadistica.y + 30;
    int separador = 40;

    char texto[200];

    for(tJugador *i = estadistica->jugador_mo; i < estadistica->jugador_mo + estadistica->ce_jugadores_mo; i++)
    {
        sprintf(texto,"%s - %d pts",i->nombre,i->Score);

        SDL_Surface *superficie = TTF_RenderText_Blended(sdl->fuente,texto,estadistica->color_texto);
        SDL_Texture *textura = SDL_CreateTextureFromSurface(sdl->renderer,superficie);

        SDL_Rect rect = {x, y + separador, superficie->w, superficie->h};
        SDL_RenderCopy(sdl->renderer,textura,NULL,&rect);

        SDL_FreeSurface(superficie);
        SDL_DestroyTexture(textura);

        separador += 40;
    }
}


void dibujarBotonesTriangulares(tSistemaSDL *sdl, tBoton_triangular* botones_triangular, size_t ce)
{
    const int triangulo[12][12] =
    {
        {T, BR, BR, T, T, T, T, T, T, T, T, T},
        {T, BR, R, BR, T, T, T, T, T, T, T, T},
        {T, BR, R, R, BR, T, T, T, T, T, T, T},
        {T, BR, R, R, R, BR, T, T, T, T, T, T},
        {T, BR, R, R, R, R, BR, T, T, T, T, T},
        {T, BR, R, R, R, R, R, BR, T, T, T, T},
        {T, BR, R, R, R, R, R, BR, T, T, T, T},
        {T, BR, R, R, R, R, BR, T, T, T, T, T},
        {T, BR, R, R, R, BR, T, T, T, T, T, T},
        {T, BR, R, R, BR, T, T, T, T, T, T, T},
        {T, BR, R, BR, T, T, T, T, T, T, T, T},
        {T, BR, BR, T, T, T, T, T, T, T, T, T}
    };

    for(int i = 0; i < ce; i++)
    {
        for(int py = 0; py < 12; py++)
        {
            for(int px = 0; px < 12; px++)
            {
                SDL_Rect pixel = {botones_triangular[i].posicion.x + px * 5, botones_triangular[i].posicion.y + py *5,5,5};

                int valor = (i == 1) ? triangulo[py][11 - px] : triangulo[py][px];

                if(valor == R)
                {
                    SDL_SetRenderDrawColor(sdl->renderer, botones_triangular[i].color_relleno.r,botones_triangular[i].color_relleno.g, botones_triangular[i].color_relleno.b, botones_triangular[i].color_relleno.a);
                    SDL_RenderFillRect(sdl->renderer, &pixel);
                }
                else if(valor == BR)
                {
                    SDL_SetRenderDrawColor(sdl->renderer, botones_triangular[i].color_bordes.r,botones_triangular[i].color_bordes.g, botones_triangular[i].color_bordes.b, botones_triangular[i].color_bordes.a);
                    SDL_RenderFillRect(sdl->renderer, &pixel);
                }
                else
                {
                    SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 0); // Negro
                    SDL_RenderFillRect(sdl->renderer, &pixel);
                }
            }
        }

    }
}
