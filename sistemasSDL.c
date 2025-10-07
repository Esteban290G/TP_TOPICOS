#include "sistemasSDL.h"
#include "sonidos.h"

void inicializarNULL(tSistemaSDL* sistema)
{
    sistema->ventana = NULL;
    sistema->renderer = NULL;
    sistema->fuente = NULL;
    sistema->fuente2 = NULL;
    sistema->fuente_titulo = NULL;
    sistema->musica_menu = NULL;
    sistema->audio_inicializado = false;
}


bool inicializarSDL(tSistemaSDL* sistema,const char* nombre_ventana, int ancho, int largo)
{
    srand(time(NULL));

    inicializarNULL(sistema);

    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0)
    {
        printf("Error al inicializar SDL_Init %s\n",SDL_GetError());
        return false;
    }

    sistema->ventana = SDL_CreateWindow(nombre_ventana,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,ancho,largo,SDL_WINDOW_SHOWN);

    if(sistema->ventana == NULL)
    {
        printf("Error al crear la ventana %s\n",SDL_GetError());
        limpiarSDL(sistema);
        return false;
    }

    sistema->renderer = SDL_CreateRenderer(sistema->ventana, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

    if(sistema->renderer == NULL)
    {
        printf("Error al crear el renderer %s\n",SDL_GetError());
        limpiarSDL(sistema);
        return false;
    }

    sistema->audio_inicializado = (Mix_OpenAudio(FREC_MUESTREO, MIX_DEFAULT_FORMAT, 2, BUFFER_AUDIO) == 0);

    if(!sistema->audio_inicializado)
    {
        printf("Error al inicializar el audio %s\n",Mix_GetError());
        limpiarSDL(sistema);
        return false;
    }
    sistema->musica_menu = Mix_LoadMUS("menu.wav");

    if(sistema->musica_menu == NULL)
    {
        printf("Error al cargar la musica %s",Mix_GetError());
        limpiarSDL(sistema);
        return false;
    }

    if(TTF_Init() == -1)
    {
        printf("Error al inicializar TTF_Init %s\n",TTF_GetError());
        limpiarSDL(sistema);
        return false;
    }

    sistema->fuente = TTF_OpenFont("8bitOperatorPlus8-Regular.ttf", 28);
    if(sistema->fuente == NULL)
    {
        printf("Error al inicializar la fuente %s\n",TTF_GetError());
        limpiarSDL(sistema);
        return false;
    }

    sistema->fuente2 = TTF_OpenFont("8bitOperatorPlus8-Regular.ttf", 35);
    if(sistema->fuente2 == NULL)
    {
        printf("Error al inicializar la fuente 2 %s\n",TTF_GetError());
        limpiarSDL(sistema);
        return false;
    }

    sistema->fuente_titulo = TTF_OpenFont("8bitOperatorPlus8-Bold.ttf",50);
    if(sistema->fuente_titulo == NULL)
    {
        printf("Error al inicializar la fuente titulo %s\n",TTF_GetError());
        limpiarSDL(sistema);
        return false;
    }

    SDL_SetRenderDrawBlendMode(sistema->renderer,SDL_BLENDMODE_BLEND);

    return true;
}

void limpiarSDL(tSistemaSDL* sistema)
{
    if(sistema->renderer)
        SDL_DestroyRenderer(sistema->renderer);
    if(sistema->ventana)
        SDL_DestroyWindow(sistema->ventana);
    if(sistema->fuente)
        TTF_CloseFont(sistema->fuente);
    if(sistema->fuente2)
        TTF_CloseFont(sistema->fuente2);
    if(sistema->fuente_titulo)
        TTF_CloseFont(sistema->fuente_titulo);
    if(sistema->musica_menu)
        Mix_FreeMusic(sistema->musica_menu);
    if(sistema->audio_inicializado)
        Mix_CloseAudio();

    TTF_Quit();
    SDL_Quit();

    inicializarNULL(sistema);

}

void reproducirMusica(tSistemaSDL* sdl)
{
    if(sdl->audio_inicializado && sdl->musica_menu)
    {
        if(Mix_PlayMusic(sdl->musica_menu, -1) == -1)
        {
            printf("Error al reproducir la musica %s",Mix_GetError());
            limpiarSDL(sdl);
        }
    }
}

void pausarMusica(tSistemaSDL* sdl)
{
    if(sdl->audio_inicializado && Mix_PlayingMusic())
    {
        Mix_PauseMusic();
    }
}

void reanudarMusica(tSistemaSDL* sdl)
{
    if (Mix_PausedMusic())
    {
        Mix_ResumeMusic();
    }
    else if (!Mix_PlayingMusic())
    {
        Mix_PlayMusic(sdl->musica_menu, -1);
    }
}

