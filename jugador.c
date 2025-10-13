#include "jugador.h"

bool _verificarMouseBotonJugar(SDL_Rect boton, int mouse_x, int mouse_y)
{
    if (mouse_x >= boton.x && mouse_x <= boton.x + boton.w && mouse_y >= boton.y && mouse_y <= boton.y + boton.h)
        return true;
    return false;
}

void inicializarPantallaJugador(tPantallaJugador *pantalla)
{
    strcpy(pantalla->Titulo, "Ingrese su nombre");
    strcpy(pantalla->texto_ingresado, "");

    pantalla->rect_ingreso_nombre = (SDL_Rect){POSICION_X - LARGO_RECT / 2, POSICION_Y - PADDING * 2, LARGO_RECT + LARGO_RECT, ANCHO_RECT};
    pantalla->input_activo = false;

    pantalla->color_fondo = (SDL_Color){0, 0, 0, 255};
    pantalla->color_titulo = (SDL_Color){255, 255, 255, 255};

    pantalla->rect_color = (SDL_Color){255, 255, 255, 255};
    pantalla->rect_color_borde = (SDL_Color){0, 0, 0, 255};

    pantalla->color_texto_ingresado = (SDL_Color){0, 0, 0, 255};

    pantalla->ce_botones = MAX_BOTONES;

    pantalla->botones[0].rectangulo = (SDL_Rect){POSICION_X, POSICION_Y + PADDING, LARGO_RECT, ANCHO_RECT};
    pantalla->botones[1].rectangulo = (SDL_Rect){POSICION_X, POSICION_Y + PADDING * 2, LARGO_RECT, ANCHO_RECT};
    pantalla->botones[2].rectangulo = (SDL_Rect){POSICION_X, POSICION_Y + PADDING * 3, LARGO_RECT, ANCHO_RECT};

    strcpy(pantalla->botones[0].texto_boton, "Modo Schonberg");
    strcpy(pantalla->botones[1].texto_boton, "Modo Mozart");
    strcpy(pantalla->botones[2].texto_boton, "Volver");

    pantalla->botones[0].valor_boton = SCHONBERG;
    pantalla->botones[1].valor_boton = MOZART;
    pantalla->botones[2].valor_boton = MENU;

    pantalla->botones[0].hover = false;
    pantalla->botones[1].hover = false;
    pantalla->botones[2].hover = false;

    pantalla->botones[0].color_texto_normal = (SDL_Color){255, 255, 255, 255};
    pantalla->botones[1].color_texto_normal = (SDL_Color){255, 255, 255, 255};
    pantalla->botones[2].color_texto_normal = (SDL_Color){255, 255, 255, 255};

    pantalla->botones[0].color_texto_hover = (SDL_Color){255, 0, 0, 255};
    pantalla->botones[1].color_texto_hover = (SDL_Color){0, 255, 0, 255};
    pantalla->botones[2].color_texto_hover = (SDL_Color){0, 0, 255, 255};
}

void inicializarJugador(tPantallaJugador *pantalla, tJugador *jugador)
{
    strcpy(jugador->nombre, pantalla->texto_ingresado);
    jugador->Score = 0;
    jugador->modo_juego = SCHONBERG;
    jugador->valorBoton = -1;
    jugador->cheat = false;
    jugador->vidas = 0;
}

void mostrarPantallaJuego(tSistemaSDL *sdl, tPantallaJugador *pantalla)
{
    colorPantalla(sdl, pantalla->color_fondo);

    SDL_SetRenderDrawColor(sdl->renderer, pantalla->rect_color.r, pantalla->rect_color.g, pantalla->rect_color.b, pantalla->rect_color.a);
    SDL_RenderFillRect(sdl->renderer, &pantalla->rect_ingreso_nombre);

    SDL_SetRenderDrawColor(sdl->renderer, pantalla->rect_color_borde.r, pantalla->rect_color_borde.g, pantalla->rect_color_borde.b, pantalla->rect_color_borde.a);
    SDL_RenderDrawRect(sdl->renderer, &pantalla->rect_ingreso_nombre);

    dibujarTextoIngresado(sdl, pantalla);

    dibujar(sdl, pantalla->botones, pantalla->ce_botones);
    mostrarTitulo(sdl, pantalla);
}

void mostrarTitulo(tSistemaSDL *sdl, tPantallaJugador *pantalla)
{
    int x, y;
    SDL_Color color;
    char *texto;

    texto = pantalla->Titulo;
    color = pantalla->color_titulo;

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

bool validarNombreJugador(const char *texto)
{
    bool caracter_valido = false;

    if (texto == NULL)
    {
        return caracter_valido;
    }

    int i = 0;
    while(texto[i] != '\0')
    {
        if(texto[i] != ' ')
        {
            caracter_valido = true;
            return caracter_valido;
        }

        i++;
    }

    return caracter_valido;
}

unsigned int controlEventosPantallaJuego(SDL_Event *evento, tPantallaJugador *pantalla, unsigned int estado_actual, tJugador *jugador)
{
    unsigned int bandera = estado_actual;

    while (SDL_PollEvent(evento))
    {
        switch (evento->type)
        {
        case SDL_MOUSEMOTION:
            for (tBoton *i = pantalla->botones; i < pantalla->botones + pantalla->ce_botones; i++)
            {
                i->hover = _verificarMouseBoton(i->rectangulo, evento->motion.x, evento->motion.y);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (evento->button.button == SDL_BUTTON_LEFT)
            {
                printf("Hiciste clic izquierdo en %d - %d\n", evento->button.x, evento->button.y);
                printf("\n");

                pantalla->input_activo = _verificarMouseBotonJugar(pantalla->rect_ingreso_nombre, evento->button.x, evento->button.y);

                for (tBoton *i = pantalla->botones; i < pantalla->botones + pantalla->ce_botones; i++)
                {
                    if (_verificarMouseBotonJugar(i->rectangulo, evento->button.x, evento->button.y))
                    {
                        if (validarNombreJugador(pantalla->texto_ingresado) || i->valor_boton == MENU)
                        {
                            printf("Hiciste clic al boton numero %d\n", i->valor_boton);
                            strcpy(jugador->nombre,pantalla->texto_ingresado);
                            bandera = i->valor_boton;
                        }
                        else
                        {
                            printf("Ingrese un nombre por favor\n");
                        }
                    }
                }
            }
            break;
        case SDL_TEXTINPUT:
            if (pantalla->input_activo)
            {
                // verificamos limite
                if (strlen(pantalla->texto_ingresado) < sizeof(pantalla->texto_ingresado) - 1)
                {
                    strcat(pantalla->texto_ingresado, evento->text.text);
                }
            }
            break;

        case SDL_KEYDOWN:
            if (pantalla->input_activo)
            {
                if (evento->key.keysym.sym == SDLK_BACKSPACE)
                {
                    int len = strlen(pantalla->texto_ingresado);
                    if (len > 0)
                    {
                        pantalla->texto_ingresado[len - 1] = '\0';
                    }
                }
                else if (evento->key.keysym.sym == SDLK_RETURN)
                {
                    pantalla->input_activo = false;
                }
            }
            break;
        }
    }
    if (bandera == MENU)
        strcpy(pantalla->texto_ingresado, "");
    return bandera;
}

void dibujarTextoIngresado(tSistemaSDL *sdl, tPantallaJugador *pantalla)
{

    if (strlen(pantalla->texto_ingresado) > 0)
    {
        TTF_Font *fuente = sdl->fuente;

        SDL_Surface *superficie = TTF_RenderText_Blended(fuente, pantalla->texto_ingresado, pantalla->color_texto_ingresado);

        if (superficie && superficie->w > 0)
        {
            SDL_Texture *textura = SDL_CreateTextureFromSurface(sdl->renderer, superficie);

            SDL_Rect rectangulo = {pantalla->rect_ingreso_nombre.x + 10, pantalla->rect_ingreso_nombre.y + (pantalla->rect_ingreso_nombre.h - superficie->h) / 2, superficie->w, superficie->h};

            SDL_RenderCopy(sdl->renderer, textura, NULL, &rectangulo);
            SDL_DestroyTexture(textura);
            SDL_FreeSurface(superficie);
        }
    }

    if (pantalla->input_activo)
    {

        SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 255);

        int ancho_total = 0;

        if (strlen(pantalla->texto_ingresado) > 0)
        {
            TTF_SizeText(sdl->fuente, pantalla->texto_ingresado, &ancho_total, NULL);
        }

        int pos_x = pantalla->rect_ingreso_nombre.x + 10 + ancho_total;

        SDL_Rect cursor = {pos_x, pantalla->rect_ingreso_nombre.y + 8, 3, pantalla->rect_ingreso_nombre.h - 16};

        SDL_RenderFillRect(sdl->renderer, &cursor);
    }
}
