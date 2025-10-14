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

void inicializartSistemaCrab(tSistemaCrab* bicho)
{
    for(int i = 0; i < MAX_CRAB; i++)
    {
        bicho->crabs[i].activo = false;
        bicho->crabs[i].transparencia = 0.0f;
    }
    bicho->ultimo_tiempo = 0;
    bicho->intervalo_creacion = 1000;
}

void crearCrab(tSistemaCrab* bicho, int ancho_pantalla, int largo_pantalla)
{
    for(int i = 0; i < MAX_CRAB; i++)
    {
        if(!bicho->crabs[i].activo)
        {
            int margen = 80;
            bicho->crabs[i].posicion.x = margen + rand() % (ancho_pantalla - PIXELES_X_LADO *3 - margen *2);
            bicho->crabs[i].posicion.y = margen + rand() % (largo_pantalla - PIXELES_X_LADO *3 - margen *2);
            bicho->crabs[i].transparencia = 0.0f;
            bicho->crabs[i].velocidad = 0.3f + (rand() % 100) / 300.0f;
            bicho->crabs[i].activo = true;
            bicho->crabs[i].tiempo = SDL_GetTicks();
        }
    }
}

void actualizarCrab(tSistemaCrab* bicho)
{
    Uint32 tiempo_actual = SDL_GetTicks();

    if(tiempo_actual - bicho->ultimo_tiempo > bicho->intervalo_creacion)
    {
        crearCrab(bicho,ANCHO,LARGO);
        bicho->ultimo_tiempo = tiempo_actual;
    }

    for(int i = 0; i < MAX_CRAB; i++)
    {
        if(bicho->crabs[i].activo)
        {
            float tiempo_vida = (tiempo_actual - bicho->crabs[i].tiempo)/1000.0f;
            bicho->crabs[i].transparencia = (sin(tiempo_vida * bicho->crabs[i].velocidad * 2)+1)/2;

            if(tiempo_actual - bicho->crabs[i].tiempo > 12000)
            {
                bicho->crabs[i].activo = false;
            }
        }

    }
}

void dibujarCrab(tSistemaSDL *sdl,tSistemaCrab* bicho)
{
    const int crab[12][12] =
    {
        {T, T, T, T, T, T, T, T, T, T, T, T},
        {T, T, T, T, T, T, T, T, T, T, T, T},
        {T, T, R, T, T, T, T, T, R, T, T, T},
        {T, T, T, R, T, T, T, R, T, T, T, T},
        {T, T, R, R, R, R, R, R, R, T, T, T},
        {T, R, R, T, R, R, R, T, R, R, T, T},
        {R, R, R, R, R, R, R, R, R, R, R, T},
        {R, T, R, R, R, R, R, R, R, T, R, T},
        {R, T, R, T, T, T, T, T, R, T, R, T},
        {T, T, T, R, R, T, R, R, T, T, T, T},
        {T, T, T, T, T, T, T, T, T, T, T, T},
        {T, T, T, T, T, T, T, T, T, T, T, T}
    };
    Uint32 tiempo_actual = SDL_GetTicks();
    float velocidad = 250.0f;
    for(int i = 0; i < MAX_CRAB; i++)
    {
        if(bicho->crabs[i].activo && bicho->crabs[i].transparencia > 0.1f)
        {
            float factor = (sin(tiempo_actual/velocidad + i) + 1) / 2; // +i para desfase entre crabs
            SDL_Color color =
            {
                (Uint8)(255 * factor),
                (Uint8)(255 * (1 - factor)),
                (Uint8)(128 + 127 * sin(tiempo_actual/600.0f + i)),
                (Uint8)(bicho->crabs[i].transparencia * 255)  // Usar transparencia completa
            };

            for(int py = 0; py < 12; py++)
            {
                for(int px = 0; px < 12; px++)
                {
                    if(crab[py][px])
                    {
                        SDL_Rect pixel = {bicho->crabs[i].posicion.x + px * 10, bicho->crabs[i].posicion.y + py *10,10,10};
                        SDL_SetRenderDrawColor(sdl->renderer, color.r,color.g, color.b, color.a);
                        SDL_RenderFillRect(sdl->renderer, &pixel);
                    }
                }
            }
        }
    }
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
