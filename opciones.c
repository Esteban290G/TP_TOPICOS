#include "opciones.h"
float _signoop(int px, int py, SDL_Point p1, SDL_Point p2)
{
    return (px - p2.x) * (p1.y - p2.y) - (p1.x - p2.x) * (py -p2.y);
}

bool _verificarclickmenu(tOpciones_despregable *menu, int mouse_x, int mouse_y)
{
    if (mouse_x >= menu->boton_despregable.x &&
            mouse_x <= menu->boton_despregable.x + menu->boton_despregable.w &&
            mouse_y >= menu->boton_despregable.y &&
            mouse_y <= menu->boton_despregable.y + menu->boton_despregable.h)
    {
        menu->abierto = !menu->abierto;
        return true;
    }
    if (menu->abierto)
    {

        for (int i = 0; i < menu->cantidad_opciones; i++)
        {
            if (mouse_x >= menu->opciones[i].rectangulo.x &&
                    mouse_x <= menu->opciones[i].rectangulo.x + menu->opciones[i].rectangulo.w &&
                    mouse_y >= menu->opciones[i].rectangulo.y &&
                    mouse_y <= menu->opciones[i].rectangulo.y + menu->opciones[i].rectangulo.h)
            {

                for (int j = 0; j < menu->cantidad_opciones; j++)
                {
                    menu->opciones[j].seleccionado = false;
                }

                menu->opciones[i].seleccionado = true;

                strcpy(menu->texto, menu->opciones[i].texto);

                menu->abierto = false;

                return true;
            }
        }

        menu->abierto = false;
        return true;
    }

    return false;
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
    strcpy(op->texto_sonidos, "Sonidos:");

    op->boton->valor_boton = MENU;
    op->boton->color_texto_hover = (SDL_Color){255,255,0,255};
    op->boton->color_texto_normal = (SDL_Color){255,255,255,255};
    op->boton->hover = false;
    op->boton->rectangulo = (SDL_Rect){30,680,LARGO_RECT,ANCHO_RECT};
    strcpy(op->boton->texto_boton,"Volver");

   // op->boton_triangular[0].color_bordes = (SDL_Color){184, 134, 11, 255};
    //op->boton_triangular[0].color_relleno = (SDL_Color){255, 200, 50, 255};
    op->boton_triangular[0].estado = SIGUIENTE;
    op->boton_triangular[0].posicion = (SDL_Point){600,180};

    //op->boton_triangular[1].color_bordes = (SDL_Color){184, 134, 11, 255};
    //op->boton_triangular[1].color_relleno = (SDL_Color){255, 200, 50, 255};
    op->boton_triangular[1].estado = ANTERIOR;
    op->boton_triangular[1].posicion = (SDL_Point){300,180};

    //op->boton_triangular[2].color_bordes = (SDL_Color){184, 134, 11, 255};
    //op->boton_triangular[2].color_relleno = (SDL_Color){255, 200, 50, 255};
    op->boton_triangular[2].estado = SIGUIENTE;
    op->boton_triangular[2].posicion = (SDL_Point){600,280};

    //op->boton_triangular[3].color_bordes = (SDL_Color){184, 134, 11, 255};
     //op->boton_triangular[3].color_relleno = (SDL_Color){255, 200, 50, 255};
    op->boton_triangular[3].estado = ANTERIOR;
    op->boton_triangular[3].posicion = (SDL_Point){300,280};


    op->boton_triangular[0].apretado = false;
    op->boton_triangular[1].apretado = false;
    op->boton_triangular[2].apretado = false;
    op->boton_triangular[3].apretado = false;

    op->boton_triangular[0].hover = false;
    op->boton_triangular[1].hover = false;
    op->boton_triangular[2].hover = false;
    op->boton_triangular[3].hover = false;

    op->fondo = (SDL_Color){0,0,0,255};

    op->config.cant_botones = 3;
    op->config.duracion_inicial = 2000;
    op->config.es_counter = false;
    op->config.es_minecraft = false;
    op->config.es_default = true;

    ///Menu despregable

    op->menu_sonido.boton_despregable = (SDL_Rect){380,500,250,40};
    strcpy(op->menu_sonido.texto,"Opciones");
    op->menu_sonido.cantidad_opciones = 3;
    op->menu_sonido.abierto = false;

    ///Opciones del menu

    op->menu_sonido.opciones[0].rectangulo = (SDL_Rect){380,540,230,35};
    strcpy(op->menu_sonido.opciones[0].texto,"Original");
    op->menu_sonido.opciones[0].seleccionado = true;

    op->menu_sonido.opciones[1].rectangulo = (SDL_Rect){380,575,230,35};
    strcpy(op->menu_sonido.opciones[1].texto,"Counter Strike");
    op->menu_sonido.opciones[1].seleccionado = false;

    op->menu_sonido.opciones[2].rectangulo = (SDL_Rect){380,610,230,35};
    strcpy(op->menu_sonido.opciones[2].texto,"Minecraft");
    op->menu_sonido.opciones[2].seleccionado = false;

}


void mostrarPantallaOpciones(tSistemaSDL* sdl, tOpciones* op)
{
    //colorPantalla(sdl,op->fondo);
    dibujarfondodegrade(sdl);
    dibujar(sdl,op->boton,CANTIDAD_BOTON_OPCIONES);
    mostrarTituloOpciones(sdl,op);
    mostrarTextos(sdl,op);
    dibujarBotonesTriangulares(sdl,op->boton_triangular,4);
    dibujarMenuDespregable(sdl,&op->menu_sonido);
    dibujarLineasSeparadoras(sdl,150);
    dibujarLineasSeparadoras(sdl,450);

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
    int x5,y5;

    char valor1[50], valor2[50];

    SDL_Color color;

    color = op->color_letras;
    char* texto_1 = op->texto_botones;
    char* texto_2 = op->texto_velocidad;
    char* texto_3 = op->texto_sonidos;

    sprintf(valor1, " %zu ", op->config.cant_botones);
    sprintf(valor2, " %.1f sg", op->config.duracion_inicial/1000);

    TTF_Font *fuente = sdl->fuente;
    TTF_Font *fuente2 = sdl->fuente2;

    SDL_Surface *superficie_1 = TTF_RenderText_Blended(fuente, texto_1, color);
    SDL_Surface *superficie_2 = TTF_RenderText_Blended(fuente, texto_2, color);
    SDL_Surface *superficie_3 = TTF_RenderText_Blended(fuente, texto_3, color);

    SDL_Surface *superficie_valor1 = TTF_RenderText_Blended(fuente2, valor1, color);
    SDL_Surface *superficie_valor2 = TTF_RenderText_Blended(fuente2, valor2, color);

    SDL_Texture *textura_1 = SDL_CreateTextureFromSurface(sdl->renderer, superficie_1);
    SDL_Texture *textura_2 = SDL_CreateTextureFromSurface(sdl->renderer, superficie_2);
    SDL_Texture *textura_3 = SDL_CreateTextureFromSurface(sdl->renderer, superficie_3);

    SDL_Texture *textura_valor1 = SDL_CreateTextureFromSurface(sdl->renderer, superficie_valor1);
    SDL_Texture *textura_valor2 = SDL_CreateTextureFromSurface(sdl->renderer, superficie_valor2);

    ///Posicion texto_1
    x1 = 10;
    y1 = 200;

    ///Posicion texto_2
    x2 = 10;
    y2 = 300;

    ///Posicion valor1
    x3 = 460;
    y3 = 200;

    ///Posicion valor2
    x4 = 428;
    y4 = 300;

    ///Posicion texto_3
    x5 = 10;
    y5 = 500;

    SDL_Rect rectangulo_1 = {x1, y1, superficie_1->w, superficie_1->h};
    SDL_Rect rectangulo_2 = {x2, y2, superficie_2->w, superficie_2->h};
    SDL_Rect rectangulo_3 = {x5,y5, superficie_3->w, superficie_3->h};
    SDL_RenderCopy(sdl->renderer, textura_1, NULL, &rectangulo_1);
    SDL_RenderCopy(sdl->renderer, textura_2, NULL, &rectangulo_2);
    SDL_RenderCopy(sdl->renderer, textura_3, NULL, &rectangulo_3);

    SDL_Rect rect_valor1 = {x3, y3, superficie_valor1->w, superficie_valor1->h};
    SDL_Rect rect_valor2 = {x4, y4, superficie_valor2->w, superficie_valor2->h};
    SDL_RenderCopy(sdl->renderer, textura_valor1, NULL, &rect_valor1);
    SDL_RenderCopy(sdl->renderer, textura_valor2, NULL, &rect_valor2);

    SDL_DestroyTexture(textura_1);
    SDL_DestroyTexture(textura_2);
    SDL_DestroyTexture(textura_3);
    SDL_DestroyTexture(textura_valor1);
    SDL_DestroyTexture(textura_valor2);
    SDL_FreeSurface(superficie_1);
    SDL_FreeSurface(superficie_2);
    SDL_FreeSurface(superficie_3);
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
            for(int i = 0; i < 4; i++)
            {
                op->boton_triangular[i].hover = _verificarTrianguloops(&op->boton_triangular[i],evento->button.x, evento->button.y);
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
                        op->boton_triangular[i].apretado = true;
                    }
                }
                configJuego->cant_botones = op->config.cant_botones;
                configJuego->duracion_inicial = op->config.duracion_inicial;

            if(_verificarclickmenu(&op->menu_sonido,evento->button.x, evento->button.y))
            {
                for(tOpciones_sonidos* i = op->menu_sonido.opciones; i < op->menu_sonido.opciones + 3; i++)
                {

                    int indice = i - op->menu_sonido.opciones;
                    if(i->seleccionado)
                    {
                        switch(indice)
                        {
                        case 0:
                            op->config.es_default = true;
                            op->config.es_counter = false;
                            op->config.es_minecraft = false;

                            break;

                        case 1:
                            op->config.es_default = false;
                            op->config.es_counter = true;
                            op->config.es_minecraft = false;
                            break;

                        case 2:
                            op->config.es_default = false;
                            op->config.es_counter = false;
                            op->config.es_minecraft = true;
                            break;
                        }
                    }
                }
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if(evento->button.button == SDL_BUTTON_LEFT)
            {
                for(int i = 0; i < 4; i++)
                {
                    op->boton_triangular[i].apretado = false;
                }
            }
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

void dibujarMenuDespregable(tSistemaSDL* sdl, tOpciones_despregable* menu_despregable)
{
    SDL_Color color_boton = {184, 134, 11, 255};
    SDL_Color color_boton_hover = {255, 215, 0, 255};
    SDL_Color color_opcion = {160, 120, 40, 255};
    SDL_Color color_opcion_seleccionada = {255, 200, 50, 255};
    SDL_Color color_texto = {255, 255, 255, 255};
    SDL_Color color_borde = {160, 120, 40, 255};


    TTF_Font* fuente = sdl->fuente;

    SDL_SetRenderDrawColor(sdl->renderer, color_boton.r, color_boton.g, color_boton.b, color_boton.a);
    SDL_RenderFillRect(sdl->renderer, &menu_despregable->boton_despregable);

    SDL_SetRenderDrawColor(sdl->renderer, color_borde.r, color_borde.g, color_borde.b, color_borde.a);
    SDL_RenderDrawRect(sdl->renderer, &menu_despregable->boton_despregable);

    SDL_Surface *superficie = TTF_RenderText_Blended(fuente,menu_despregable->texto, color_texto);
    SDL_Texture *textura = SDL_CreateTextureFromSurface(sdl->renderer,superficie);

    SDL_Rect rect_texto = {menu_despregable->boton_despregable.x + 10,
    menu_despregable->boton_despregable.y +(menu_despregable->boton_despregable.h - superficie->h)/2,
    superficie->w, superficie->h};

    SDL_RenderCopy(sdl->renderer,textura,NULL,&rect_texto);

    SDL_DestroyTexture(textura);
    SDL_FreeSurface(superficie);

    int triangulo_size = 8;
    SDL_Point triangulo[3];

    if(menu_despregable->abierto)
    {
        triangulo[0] = (SDL_Point){menu_despregable->boton_despregable.x + menu_despregable->boton_despregable.w - 20,
                                  menu_despregable->boton_despregable.y + menu_despregable->boton_despregable.h/2 - triangulo_size/2};
        triangulo[1] = (SDL_Point){menu_despregable->boton_despregable.x + menu_despregable->boton_despregable.w - 10,
                                  menu_despregable->boton_despregable.y + menu_despregable->boton_despregable.h/2 - triangulo_size/2};
        triangulo[2] = (SDL_Point){menu_despregable->boton_despregable.x + menu_despregable->boton_despregable.w - 15,
                                  menu_despregable->boton_despregable.y + menu_despregable->boton_despregable.h/2 + triangulo_size/2};
    }
    else
    {
        triangulo[0] = (SDL_Point){menu_despregable->boton_despregable.x + menu_despregable->boton_despregable.w - 20,
                                  menu_despregable->boton_despregable.y + menu_despregable->boton_despregable.h/2 + triangulo_size/2};
        triangulo[1] = (SDL_Point){menu_despregable->boton_despregable.x + menu_despregable->boton_despregable.w - 10,
                                  menu_despregable->boton_despregable.y + menu_despregable->boton_despregable.h/2 + triangulo_size/2};
        triangulo[2] = (SDL_Point){menu_despregable->boton_despregable.x + menu_despregable->boton_despregable.w - 15,
                                  menu_despregable->boton_despregable.y + menu_despregable->boton_despregable.h/2 - triangulo_size/2};

    }

    SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 255);
    SDL_RenderDrawLines(sdl->renderer,triangulo,3);

    if(menu_despregable->abierto)
    {
        for(int i = 0; i < menu_despregable->cantidad_opciones; i++)
        {
            if (menu_despregable->opciones[i].seleccionado)
            {
                SDL_SetRenderDrawColor(sdl->renderer,
                                       color_opcion_seleccionada.r,
                                       color_opcion_seleccionada.g,
                                       color_opcion_seleccionada.b,
                                       color_opcion_seleccionada.a);
            }
            else
            {
                SDL_SetRenderDrawColor(sdl->renderer,
                                       color_opcion.r,
                                       color_opcion.g,
                                       color_opcion.b,
                                       color_opcion.a);
            }

            SDL_RenderFillRect(sdl->renderer, &menu_despregable->opciones[i].rectangulo);

            SDL_SetRenderDrawColor(sdl->renderer, color_borde.r, color_borde.g, color_borde.b, color_borde.a);
            SDL_RenderDrawRect(sdl->renderer, &menu_despregable->opciones[i].rectangulo);

            superficie = TTF_RenderText_Blended(fuente, menu_despregable->opciones[i].texto, color_texto);
            textura = SDL_CreateTextureFromSurface(sdl->renderer, superficie);

            rect_texto = (SDL_Rect)
            {
                menu_despregable->opciones[i].rectangulo.x + 10,
                     menu_despregable->opciones[i].rectangulo.y + (menu_despregable->opciones[i].rectangulo.h - superficie->h) / 2,
                     superficie->w,
                     superficie->h
            };
            SDL_RenderCopy(sdl->renderer, textura, NULL, &rect_texto);

            SDL_DestroyTexture(textura);
            SDL_FreeSurface(superficie);

            if (menu_despregable->opciones[i].seleccionado)
            {
                SDL_SetRenderDrawColor(sdl->renderer, 255, 255, 255, 255);
                SDL_Rect punto =
                {
                    menu_despregable->opciones[i].rectangulo.x + 5,
                    menu_despregable->opciones[i].rectangulo.y + menu_despregable->opciones[i].rectangulo.h/2 - 2,
                    4, 4
                };
                SDL_RenderFillRect(sdl->renderer, &punto);
            }
        }
    }
}

void dibujarfondodegrade(tSistemaSDL*sdl)
{
    SDL_Color color_inicio = {50, 0, 0, 255};
    SDL_Color color_fin    = {120, 0, 0, 255};
    for (int y = 0; y < LARGO; y++)
    {
        float t = (float)y / LARGO;
        Uint8 r = (Uint8)(color_inicio.r + t * (color_fin.r - color_inicio.r));
        Uint8 g = (Uint8)(color_inicio.g + t * (color_fin.g - color_inicio.g));
        Uint8 b = (Uint8)(color_inicio.b + t * (color_fin.b - color_inicio.b));

        SDL_SetRenderDrawColor(sdl->renderer, r, g, b, 255);
        SDL_RenderDrawLine(sdl->renderer, 0, y, ANCHO, y);
    }
}
void dibujarLineasSeparadoras(tSistemaSDL*sdl, int y)
{
    SDL_Color color_linea = {184, 134, 11, 255};
    SDL_SetRenderDrawColor(sdl->renderer, color_linea.r, color_linea.g, color_linea.b, color_linea.a);
    SDL_RenderDrawLine(sdl->renderer, 50, y, ANCHO - 40, y);
}

