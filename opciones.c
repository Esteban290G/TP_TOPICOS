#include "opciones.h"
float _signoop(int px, int py, SDL_Point p1, SDL_Point p2)
{
    return (px - p2.x) * (p1.y - p2.y) - (p1.x - p2.x) * (py -p2.y);
}

bool _verificarTrianguloops(tBoton_triangular* boton_triangular, int mouse_x, int mouse_y)
{
    SDL_Point vertices[3];
    int ancho_triangulo = 12 *5;
    int alto_triangulo = 12 *5;

    bool invertir = (boton_triangular->posicion.x > 400);

    if(invertir)
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

    d1 = _signoop(mouse_x, mouse_y, vertices[0], vertices[1]);
    d2 = _signoop(mouse_x, mouse_y, vertices[1], vertices[2]);
    d3 = _signoop(mouse_x, mouse_y, vertices[2], vertices[0]);


    negativo = (d1 < 0) || (d2 < 0) || (d3 < 0);
    positivo = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(negativo && positivo);
}

void inicializarOpciones(tOpciones *op)
{
    strcpy(op->Titulo,"Opciones");
    op->color_letras = (SDL_Color){255,255,255,255};

    strcpy(op->texto_botones,"Cantidad botones:");
    strcpy(op->texto_velocidad,"Duracion:");

    op->boton->valor_boton = MENU;
    op->boton->color_texto_hover = (SDL_Color){255,255,0,255};
    op->boton->color_texto_normal = (SDL_Color){255,255,255,255};
    op->boton->hover = false;
    op->boton->rectangulo = (SDL_Rect){30,680,LARGO_RECT,ANCHO_RECT};
    strcpy(op->boton->texto_boton,"Volver");

    op->boton_triangular[0].color_bordes = (SDL_Color){255,255,0,255};
    op->boton_triangular[0].color_relleno = (SDL_Color){255,255,255,255};
    op->boton_triangular[0].estado = SIGUIENTE;
    op->boton_triangular[0].posicion = (SDL_Point){600,180};

    op->boton_triangular[1].color_bordes = (SDL_Color){255,255,0,255};
    op->boton_triangular[1].color_relleno = (SDL_Color){255,255,255,255};
    op->boton_triangular[1].estado = ANTERIOR;
    op->boton_triangular[1].posicion = (SDL_Point){300,180};

    op->boton_triangular[2].color_bordes = (SDL_Color){255,255,0,255};
    op->boton_triangular[2].color_relleno = (SDL_Color){255,255,255,255};
    op->boton_triangular[2].estado = SIGUIENTE;
    op->boton_triangular[2].posicion = (SDL_Point){600,280};

    op->boton_triangular[3].color_bordes = (SDL_Color){255,255,0,255};
    op->boton_triangular[3].color_relleno = (SDL_Color){255,255,255,255};
    op->boton_triangular[3].estado = ANTERIOR;
    op->boton_triangular[3].posicion = (SDL_Point){300,280};

    op->fondo = (SDL_Color){0,0,0,255};

    op->config.cant_botones = 3;
    op->config.duracion_inicial = 2000;
}


void mostrarPantallaOpciones(tSistemaSDL* sdl, tOpciones* op)
{
    colorPantalla(sdl,op->fondo);
    dibujar(sdl,op->boton,CANTIDAD_BOTON_OPCIONES);
    mostrarTituloOpciones(sdl,op);
    mostrarTextos(sdl,op);
    dibujarBotonesTriangularess(sdl,op->boton_triangular,4);

}

void mostrarTituloOpciones(tSistemaSDL* sdl, tOpciones* op)
{
    int x, y;
    SDL_Color color;
    char *texto;

    texto = op->Titulo;
    color = op->color_letras;

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

void mostrarTextos(tSistemaSDL* sdl, tOpciones* op)
{
    int x1,x2,y1,y2;
    int x3,x4,y3,y4;

    char valor1[50], valor2[50];

    SDL_Color color;

    color = op->color_letras;
    char* texto_1 = op->texto_botones;
    char* texto_2 = op->texto_velocidad;

    sprintf(valor1, " %lld ", op->config.cant_botones);
    sprintf(valor2, " %.1f sg", op->config.duracion_inicial);

    TTF_Font *fuente = sdl->fuente;
    TTF_Font *fuente2 = sdl->fuente2;

    SDL_Surface *superficie_1 = TTF_RenderText_Blended(fuente, texto_1, color);
    SDL_Surface *superficie_2 = TTF_RenderText_Blended(fuente, texto_2, color);

    SDL_Surface *superficie_valor1 = TTF_RenderText_Blended(fuente2, valor1, color);
    SDL_Surface *superficie_valor2 = TTF_RenderText_Blended(fuente2, valor2, color);

    SDL_Texture *textura_1 = SDL_CreateTextureFromSurface(sdl->renderer, superficie_1);
    SDL_Texture *textura_2 = SDL_CreateTextureFromSurface(sdl->renderer, superficie_2);

    SDL_Texture *textura_valor1 = SDL_CreateTextureFromSurface(sdl->renderer, superficie_valor1);
    SDL_Texture *textura_valor2 = SDL_CreateTextureFromSurface(sdl->renderer, superficie_valor2);

    x1 = 10;
    y1 = 200;

    x2 = 10;
    y2 = 300;

    x3 = 460;
    y3 = 200;

    x4 = 428;
    y4 = 300;

    SDL_Rect rectangulo_1 = {x1, y1, superficie_1->w, superficie_1->h};
    SDL_Rect rectangulo_2 = {x2, y2, superficie_2->w, superficie_2->h};
    SDL_RenderCopy(sdl->renderer, textura_1, NULL, &rectangulo_1);
    SDL_RenderCopy(sdl->renderer, textura_2, NULL, &rectangulo_2);

    SDL_Rect rect_valor1 = {x3, y3, superficie_valor1->w, superficie_valor1->h};
    SDL_Rect rect_valor2 = {x4, y4, superficie_valor2->w, superficie_valor2->h};
    SDL_RenderCopy(sdl->renderer, textura_valor1, NULL, &rect_valor1);
    SDL_RenderCopy(sdl->renderer, textura_valor2, NULL, &rect_valor2);

    SDL_DestroyTexture(textura_1);
    SDL_DestroyTexture(textura_2);
    SDL_DestroyTexture(textura_valor1);
    SDL_DestroyTexture(textura_valor2);
    SDL_FreeSurface(superficie_1);
    SDL_FreeSurface(superficie_2);
    SDL_FreeSurface(superficie_valor1);
    SDL_FreeSurface(superficie_valor2);
}

unsigned int eventosOpciones(SDL_Event* evento, tOpciones *op, unsigned int estado_actual, tConfigJuego *configJuego)
{

   unsigned int bandera = estado_actual;
    while(SDL_PollEvent(evento))
    {
        switch(evento->type)
        {
        case SDL_MOUSEMOTION:
            for(tBoton* i = op->boton; i< op->boton + CANTIDAD_BOTON_OPCIONES; i++)
            {
                i->hover = _verificarMouseBoton(i->rectangulo,evento->motion.x, evento->motion.y);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(evento->button.button == SDL_BUTTON_LEFT)
            {
                printf("Hiciste clic izquierdo en %d - %d\n",evento->button.x,evento->button.y);
                printf("\n");

                for(tBoton*i = op->boton; i < op->boton + CANTIDAD_BOTON_OPCIONES; i++)
                {
                    if(_verificarMouseBoton(i->rectangulo,evento->button.x,evento->button.y))
                    {
                        printf("\nHiciste clic al boton numero %d\n",i->valor_boton);
                        bandera = i->valor_boton;}
                    }
                }

                for(int i = 0; i < 4; i++)
                {
                    if(_verificarTrianguloops(&op->boton_triangular[i],evento->button.x, evento->button.y))
                    {
                        printf("Hiciste click en el triangulo\n");

                        switch(i)
                        {
                        case 0:
                            if(op->config.cant_botones < 8)
                                op->config.cant_botones ++;
                            break;
                        case 1:
                            if(op->config.cant_botones > 3)
                                op->config.cant_botones --;
                            break;
                        case 2:
                            if(op->config.duracion_inicial < 5000)
                                op->config.duracion_inicial += 1000;
                            break;
                        case 3:
                            if(op->config.duracion_inicial > 2000)
                                op->config.duracion_inicial -= 1000;
                            break;
                        }
                    }
                }
                configJuego->cant_botones = op->config.cant_botones;
                configJuego->duracion_inicial = op->config.duracion_inicial;


            break;

        case SDL_QUIT:
            bandera = SALIR;
            printf("\nEstado actual: Menu Saliendo\n");
            break;
        }
    }
    if(bandera == SIGUIENTE || bandera == ANTERIOR)
        bandera = estado_actual;
    return bandera;
}

void dibujarBotonesTriangularess(tSistemaSDL *sdl, tBoton_triangular* botones_triangular, size_t ce)
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
