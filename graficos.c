#include "graficos.h"

void colorPantalla(tSistemaSDL* sdl, SDL_Color color)
{
    SDL_SetRenderDrawColor(sdl->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(sdl->renderer);
}

void dibujar(tSistemaSDL* sdl, tBoton* botonRect, size_t ce)
{
    for(tBoton* i = botonRect; i < botonRect + ce; i ++)
    {
        renderizarTexto(sdl,i);
    }
}

void renderizarTexto(tSistemaSDL* sdl,tBoton* boton)
{
    int x,y;
    TTF_Font* fuente_actual = NULL;
    SDL_Color color;

    if(!boton->hover)
    {
        fuente_actual = sdl->fuente;
        color = boton->color_texto_normal;
    }
    else
    {
        fuente_actual = sdl->fuente2;
        color = boton->color_texto_hover;
    }

    SDL_Surface* superficie = TTF_RenderText_Blended(fuente_actual,boton->texto_boton,color);

    if(superficie == NULL)
    {
        printf("Error al crear la superficie %s",SDL_GetError());
        return;
    }

    SDL_Texture* textura = SDL_CreateTextureFromSurface(sdl->renderer,superficie);

    if(textura == NULL)
    {
        printf("Error al crear la textura %s",SDL_GetError());
        return;
    }

    x = boton->rectangulo.x + (boton->rectangulo.w - superficie->w)/2;
    y = boton->rectangulo.y + (boton->rectangulo.h - superficie->h)/2;

    SDL_Rect rectangulo_txt = {x,y,superficie->w,superficie->h};

    SDL_RenderCopy(sdl->renderer,textura,NULL,&rectangulo_txt);

    SDL_DestroyTexture(textura);
    SDL_FreeSurface(superficie);
}

void dibujarBotonSprite(SDL_Renderer *renderer, const int sprite[15][15], int x, int y, SDL_Color colorBase, int tamPixel)
{
    SDL_Color color;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for(int py = 0; py < 15; py++)
    {
        for(int px = 0; px < 15; px++)
        {
            int valor = sprite[py][px];

            switch(valor)
            {
            case T:
                color = (SDL_Color){0, 0, 0, 0};
                break;
            case BA:
                color = colorBase;
                break;
            case L:
                color.r = (colorBase.r + 50 > 255) ? 255 : colorBase.r + 50;
                color.g = (colorBase.g + 50 > 255) ? 255 : colorBase.g + 50;
                color.b = (colorBase.b + 50 > 255) ? 255 : colorBase.b + 50;
                color.a = colorBase.a;
                break;
            case S:
                color.r = (colorBase.r - 50 < 0) ? 0 : colorBase.r - 50;
                color.g = (colorBase.g - 50 < 0) ? 0 : colorBase.g - 50;
                color.b = (colorBase.b - 50 < 0) ? 0 : colorBase.b - 50;
                color.a = colorBase.a;
                break;
            case W:
                color = (SDL_Color){255,255,255,255};
                break;
            }

            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_Rect pixel = {x + px * tamPixel, y + py * tamPixel, tamPixel, tamPixel};
            SDL_RenderFillRect(renderer, &pixel);
        }
    }
}
