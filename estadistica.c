#include "estadistica.h"

bool cargarTopDesdeArchivo(tJugador *vec, size_t *ce, const char *nombre_archivo)
{
    FILE *pf = fopen(nombre_archivo, "rb");
    if (!pf)
    {
        *ce = 0;
        return false;
    }

    *ce = fread(vec, sizeof(tJugador), MAX_JUGADORES, pf);
    fclose(pf);
    return true;
}

void guardarTopEnArchivo(tJugador *vec, size_t ce, const char *nombre_archivo)
{
    FILE *pf = fopen(nombre_archivo, "wb");
    if (!pf)
    {
        perror("Error al guardar el archivo de top");
        return;
    }

    fwrite(vec, sizeof(tJugador), ce, pf);
    fclose(pf);
}

void insertarEnTop(tJugador *vec, size_t *ce, tJugador nuevo)
{
    // Si el vector está vacío
    if (*ce == 0)
    {
        vec[0] = nuevo;
        (*ce)++;
        return;
    }

    // Si hay menos de 5 jugadores, insertar normalmente
    if (*ce < MAX_JUGADORES)
    {
        vec[*ce] = nuevo;
        (*ce)++;
    }
    else if (nuevo.Score > vec[*ce - 1].Score)
    {
        vec[*ce - 1] = nuevo; // reemplaza al último
    }

    // Ordenar de mayor a menor
    for (int i = 0; i < *ce - 1; i++)
    {
        for (int j = i + 1; j < *ce; j++)
        {
            if (vec[j].Score > vec[i].Score)
            {
                tJugador aux = vec[i];
                vec[i] = vec[j];
                vec[j] = aux;
            }
        }
    }
}

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

    //estadistica->botones_triangulares[0].color_relleno = (SDL_Color){255,255,255,255};
    //estadistica->botones_triangulares[1].color_relleno = (SDL_Color){255,255,255,255};

    //estadistica->botones_triangulares[0].color_bordes = (SDL_Color){255,255,0,255};
    //estadistica->botones_triangulares[1].color_bordes = (SDL_Color){255,255,0,255};

    estadistica->botones_triangulares[0].apretado = false;
    estadistica->botones_triangulares[1].apretado = false;

    estadistica->botones_triangulares[0].hover = false;
    estadistica->botones_triangulares[1].hover = false;


    estadistica->botones_triangulares[0].posicion = (SDL_Point){500,580};
    estadistica->botones_triangulares[1].posicion = (SDL_Point){200,580};

    estadistica->botones_triangulares[0].estado = MOZART;
    estadistica->botones_triangulares[1].estado = SCHONBERG;

    estadistica->rec_estadistica = (SDL_Rect)
    {
        180,
        180,
        LARGO_RECT * 2,
        ANCHO_RECT * 7
    };

    strcpy(estadistica->titulo,"Estadistica");

    ///AUX

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

    x = (ANCHO - superficie->w) / 2;
    y = 600;

    SDL_Rect rectangulo = {x, y, superficie->w, superficie->h};
    SDL_RenderCopy(sdl->renderer, textura, NULL, &rectangulo);

    SDL_DestroyTexture(textura);
    SDL_FreeSurface(superficie);
}

void mostrarPantallaEstadistica(tSistemaSDL*sdl ,tEstadistica* estadistica)
{
    SDL_SetRenderDrawBlendMode(sdl->renderer, SDL_BLENDMODE_BLEND);
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
    SDL_SetRenderDrawBlendMode(sdl->renderer, SDL_BLENDMODE_NONE);
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
            for(int i = 0; i < 4; i++)
            {
                estadistica->botones_triangulares[i].hover = _verificarTriangulo(&estadistica->botones_triangulares[i],evento->button.x, evento->button.y);
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

                        estadistica->botones_triangulares[i].apretado = true;
                    }
                }

            }
            break;

        case SDL_MOUSEBUTTONUP:
            if(evento->button.button == SDL_BUTTON_LEFT)
            {
                for(int i = 0; i < 4; i++)
                {
                    estadistica->botones_triangulares[i].apretado = false;
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
        sprintf(texto,"%-15s %6d pts",i->nombre,i->Score);

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
        sprintf(texto,"%-15s  %3d pts",i->nombre,i->Score);

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

                bool invertir = (i % 2 == 1);

                int valor = invertir ? triangulo[py][11 - px] : triangulo[py][px];

                if(valor == R)
                {
                    if(botones_triangular[i].apretado == true)
                    {
                        botones_triangular[i].color_relleno = (SDL_Color){184, 134, 11, 255};
                    }
                    else if(botones_triangular[i].hover == true)
                    {
                        botones_triangular[i].color_relleno = (SDL_Color){255, 225, 100, 255};
                    }
                    else
                    {
                        botones_triangular[i].color_relleno = (SDL_Color){255, 200, 50, 255};
                    }
                    SDL_SetRenderDrawColor(sdl->renderer, botones_triangular[i].color_relleno.r,botones_triangular[i].color_relleno.g, botones_triangular[i].color_relleno.b, botones_triangular[i].color_relleno.a);
                    SDL_RenderFillRect(sdl->renderer, &pixel);
                }
                else if(valor == BR)
                {
                    if(botones_triangular[i].apretado == true)
                    {
                        botones_triangular[i].color_bordes = (SDL_Color){139, 101, 8, 255};
                    }
                    else if(botones_triangular[i].hover == true)
                    {
                        botones_triangular[i].color_bordes = (SDL_Color){218, 165, 32, 255};
                    }
                    else
                    {
                         botones_triangular[i].color_bordes = (SDL_Color){184, 134, 11, 255};
                    }

                    SDL_SetRenderDrawColor(sdl->renderer, botones_triangular[i].color_bordes.r,botones_triangular[i].color_bordes.g, botones_triangular[i].color_bordes.b, botones_triangular[i].color_bordes.a);
                    SDL_RenderFillRect(sdl->renderer, &pixel);
                }
            }
        }

    }
}
