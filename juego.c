#include "juego.h"
#include "menus.h"
#include "graficos.h"

int generarAleatorio(int num_min, int num_max)
{
    int valor = rand() % (num_max - num_min + 1) + num_min;

    return valor;
}

void dibujarBoton(tSistemaSDL *sdl, tBotonSimon *boton, size_t cant_botones)
{
    for (tBotonSimon *i = boton; i < boton + cant_botones; i++)
    {
        SDL_SetRenderDrawColor(sdl->renderer, i->color_base.r, i->color_base.g, i->color_base.b, i->color_base.a);
        SDL_RenderFillRect(sdl->renderer, &i->rectangulo);
    }
}

void dibujarPantallaJuego(tSistemaSDL *sdl, SDL_Color color, tBotonSimon *boton_simon, size_t cant_botones, tBoton *boton_normal, size_t ce_normal)
{
    colorPantalla(sdl, color);
    dibujar(sdl, boton_normal, ce_normal);
    dibujarBoton(sdl, boton_simon, cant_botones);
}

void cargarBotonSimon(tBotonSimon *boton_simon, SDL_Color *color_1, SDL_Color *color_2, size_t cant_botones, int *v_valor)
{
    int cant_filas = 3;
    int cant_columnas = 3;
    int espacio = 20;
    int pos_x_inicial = ANCHO / 9;
    int pos_y_inicial = LARGO / 8;

    SDL_Point posiciones[3][3];

    for (int fila = 0; fila < cant_filas; fila++)
    {
        for (int col = 0; col < cant_columnas; col++)
        {
            posiciones[fila][col].x = pos_x_inicial + col * (ANCHO_SIMON + espacio);
            posiciones[fila][col].y = pos_y_inicial + fila * (ANCHO_SIMON + espacio);
        }
    }

    SDL_Point pos_activas[8];
    int total_activados = 0;

    switch (cant_botones)
    {
    case 3:
    {
        SDL_Point temp[] = {posiciones[0][1], posiciones[2][0], posiciones[2][2]};

        total_activados = 3;
        for (int i = 0; i < total_activados; i++)
        {
            pos_activas[i] = temp[i];
        }
        break;
    }

    case 4:
    {
        SDL_Point temp[] = {posiciones[0][0], posiciones[0][2], posiciones[2][0], posiciones[2][2]};

        total_activados = 4;
        for (int i = 0; i < total_activados; i++)
        {
            pos_activas[i] = temp[i];
        }
        break;
    }

    case 5:
    {
        SDL_Point temp[] = {posiciones[0][0], posiciones[0][2], posiciones[2][0], posiciones[2][2], posiciones[1][1]};

        total_activados = 5;
        for (int i = 0; i < total_activados; i++)
        {
            pos_activas[i] = temp[i];
        }
        break;
    }
    case 6:
    {
        SDL_Point temp[] = {posiciones[0][0], posiciones[0][1], posiciones[0][2], posiciones[2][0], posiciones[2][1], posiciones[2][2]};

        total_activados = 6;
        for (int i = 0; i < total_activados; i++)
        {
            pos_activas[i] = temp[i];
        }
        break;
    }
    case 7:
    {
        SDL_Point temp[] = {posiciones[0][0], posiciones[0][1], posiciones[0][2], posiciones[1][1], posiciones[2][0], posiciones[2][1], posiciones[2][2]};

        total_activados = 7;
        for (int i = 0; i < total_activados; i++)
        {
            pos_activas[i] = temp[i];
        }
        break;
    }
    case 8:
    {
        SDL_Point temp[] = {posiciones[0][0], posiciones[0][1], posiciones[0][2], posiciones[1][0], posiciones[1][2], posiciones[2][0], posiciones[2][1], posiciones[2][2]};

        total_activados = 8;
        for (int i = 0; i < total_activados; i++)
        {
            pos_activas[i] = temp[i];
        }
        break;
    }
    }

    for (size_t i = 0; i < cant_botones && i < total_activados; i++)
    {
        boton_simon[i].rectangulo = (SDL_Rect)
        {
            pos_activas[i].x, pos_activas[i].y, LARGO_SIMON, ANCHO_SIMON
        };
        boton_simon[i].color_base = color_1[i];
        boton_simon[i].color_sonando = color_2[i];
        boton_simon[i].color_aux = color_1[i];
        boton_simon[i].valor_boton = v_valor[i];
        boton_simon[i].tiempo_ultimo_sonido = 0;
        boton_simon[i].sonando = false;
    }
}

int inicializarSecuencia(tSecuencia *secuencia, size_t cant_botones)
{
    secuencia->longitud = LONG_INICIAL;
    secuencia->tiempo_acumulado = 0;
    secuencia->indice = 0;
    secuencia->mostrando = true;
    secuencia->activo = false;
    secuencia->reproduciendo = true;
    secuencia->primera_vez = false;

    secuencia->vecSecuencia = malloc(sizeof(int) * secuencia->longitud);
    if (!secuencia->vecSecuencia)
    {
        printf("Error de memoria.\n");
        return MEM_ERROR;
    }

    secuencia->vecSecuencia[0] = generarAleatorio(N_MIN, cant_botones - 1);

    return MEM_OK;
}

int agregarElemSecuencia(tSecuencia *secuencia, size_t cant_botones)
{
    secuencia->longitud++;

    int *aux = realloc(secuencia->vecSecuencia, sizeof(int) * secuencia->longitud);
    if (!aux)
    {
        free(secuencia->vecSecuencia);
        printf("Error de memoria.\n");
        return MEM_ERROR;
    }

    secuencia->vecSecuencia = aux;

    secuencia->vecSecuencia[secuencia->longitud - 1] = generarAleatorio(N_MIN, cant_botones - 1);

    return MEM_OK;
}

void reproducirSecuencia(tSistemaSDL *sdl, Mix_Chunk *sonidos[], tBotonSimon *boton_simon, size_t cant_botones, SDL_Color color, tBoton *boton_normal, size_t ce_normal, float deltaTime, tSecuencia *secuencia, float duracion_inicial, int limite_mozart)
{
    if (!secuencia->reproduciendo)
    {
        return;
    }

    int j;
    secuencia->tiempo_acumulado += deltaTime;

    float tiempoPrendidoBase = duracion_inicial * 0.7f;
    float tiempoApagadoBase = duracion_inicial * 0.3f;
    float tiempoEsperaInicio = 1500;

    // printf("\nTiempo original = %.2f \nTiempo Prendio = %.2f \nTiempoApagado = %.2f",duracion_inicial,tiempoPrendidoBase,tiempoApagadoBase);

    float acelerador = 0.03f * (secuencia->longitud - 1);

    // un tope para que no se pase a negativo la resta
    if (acelerador > 1.0f)
    {
        acelerador = 0.9f;
    }

    float tiempoPrendido = tiempoPrendidoBase * (1.0f - acelerador);
    float tiempoApagado = tiempoApagadoBase * (1.0f - acelerador);

    // printf("\ntiempo prendido actual = %.2f \ntiempo apagado actual = %.2f",tiempoPrendido, tiempoApagado);

    if (secuencia->primer_boton && secuencia->tiempo_acumulado <= tiempoEsperaInicio)
    {
        return;
    }

    int tope;
    if (limite_mozart > 0 && limite_mozart <= secuencia->longitud)
    {
        tope = limite_mozart;
    }
    else
    {
        tope = secuencia->longitud;
    }

    if (!secuencia->activo && secuencia->indice < tope) // primero para prender el boton si no hay ninguno prendido
    {
        j = secuencia->vecSecuencia[secuencia->indice];
        Mix_PlayChannel(1, sonidos[j], 0);
        boton_simon[j].color_base = boton_simon[j].color_sonando;
        secuencia->activo = true;
        secuencia->tiempo_acumulado = 0;
        secuencia->primer_boton = false;
    }
    else if (secuencia->activo && secuencia->mostrando && secuencia->tiempo_acumulado >= tiempoPrendido) // apagar el boton despues del tiempo prendido
    {
        j = secuencia->vecSecuencia[secuencia->indice];
        boton_simon[j].color_base = boton_simon[j].color_aux;
        secuencia->mostrando = false;
        secuencia->tiempo_acumulado = 0;
    }
    else if (secuencia->activo && !secuencia->mostrando && secuencia->tiempo_acumulado >= tiempoApagado) // cooldown para esperar un rato antes de pasar al siguiente
    {
        secuencia->indice++;
        secuencia->mostrando = true;
        secuencia->activo = false;
        secuencia->tiempo_acumulado = 0;

        if (secuencia->indice >= tope)
        {
            secuencia->indice = 0;
            secuencia->reproduciendo = false;
            secuencia->primer_boton = true;
        }
    }
    dibujarPantallaJuego(sdl, color, boton_simon, cant_botones, boton_normal, ce_normal);
}

unsigned int controlEventosSimon(SDL_Event *evento, tBotonSimon *boton_simon, size_t cant_botones, unsigned int estado_actual, tBoton *boton_normal, size_t ce_normal, Mix_Chunk *sonidos[], tSecuencia *secuencia, float deltaTime, tJugador *jugador)
{
    int i_sonido = 0;
    unsigned int estado = estado_actual;

    boton_simon->tiempo_ultimo_sonido += deltaTime; // para que haya un tiempo entre clics entre CUALQUIER boton
    float tiempo_espera = 500;

    while (SDL_PollEvent(evento))
    {
        switch (evento->type)
        {
        case SDL_MOUSEBUTTONDOWN:
            if (evento->button.button == SDL_BUTTON_LEFT)
            {
                printf("Hiciste clic izquierdo en %d - %d\n", evento->button.x, evento->button.y);
                printf("\n");

                for (tBotonSimon *i = boton_simon; i < boton_simon + cant_botones; i++)
                {
                    if (_verificarMouseBoton(i->rectangulo, evento->button.x, evento->button.y))
                    {
                        if (boton_simon->tiempo_ultimo_sonido >= tiempo_espera && !secuencia->reproduciendo && !i->sonando)
                        {
                            Mix_PlayChannel(1, sonidos[i_sonido], 0);
                            boton_simon->tiempo_ultimo_sonido = 0;

                            printf("\nHiciste clic al boton numero %d\n", i->valor_boton);
                            i->color_base = i->color_sonando;
                            i->sonando = true;

                            jugador->valorBoton = i->valor_boton;
                        }
                    }
                    i_sonido++;
                }
                for (tBoton *i = boton_normal; i < boton_normal + ce_normal; i++)
                {
                    if (_verificarMouseBoton(i->rectangulo, evento->button.x, evento->button.y))
                    {
                        printf("\nHiciste clic al boton numero %d\n", i->valor_boton);
                        estado = i->valor_boton;
                    }
                }
            }
            break;

        case SDL_MOUSEMOTION:
            for (tBoton *i = boton_normal; i < boton_normal + ce_normal; i++)
            {
                i->hover = _verificarMouseBoton(i->rectangulo, evento->motion.x, evento->motion.y);
            }
            break;

        case SDL_MOUSEBUTTONUP:

            if (evento->button.button == SDL_BUTTON_LEFT)
            {
                for (tBotonSimon *i = boton_simon; i < boton_simon + cant_botones; i++)
                {
                    i->color_base = i->color_aux;
                    i->sonando = false;
                }
            }
            break;

        case SDL_QUIT:
            estado = SALIR;
            printf("\nEstado actual: Menu Saliendo\n");
            break;
        }
    }

    return estado;
}

bool validarJugador(tJugador *jugador, tSecuencia *sec)
{
    bool correcto = false;

    if (jugador->valorBoton != -1)
    {
        if (sec->vecSecuencia[sec->indice] == jugador->valorBoton)
        {
            printf("CORRECTO\n");
            correcto = true;
            sec->indice++;
        }
        else
        {
            printf("INCORRECTO\n");
        }
    }

    return correcto;
}

void reiniciarJuego(tSecuencia *sec)
{
    if (sec->primera_vez)
    {
        free(sec->vecSecuencia);
    }
    sec->vecSecuencia = NULL;
    sec->longitud = LONG_INICIAL;
    sec->indice = 0;
    sec->primera_vez = true;
    sec->primer_boton = true;
}

// Mozart

void inicializarSecuenciaMozart(tSecuencia *secuencia, size_t cant_elem)
{
    secuencia->indice = 0;
    secuencia->tiempo_acumulado = 0;
    secuencia->longitud = cant_elem;
    secuencia->mostrando = true;
    secuencia->activo = false;
    secuencia->reproduciendo = true;
    secuencia->primera_vez = false;
    secuencia->mozart_actual = 1;
}

int copiarSecuenciaMozart(tSecuencia *vec, char *nombre_archivo, size_t cant_elem)
{
    vec->vecSecuencia = malloc(sizeof(int) * cant_elem);
    if (!vec->vecSecuencia)
    {
        printf("Error de memoria.\n");
        return MEM_ERROR;
    }

    int i = 0;

    FILE *pf = fopen(nombre_archivo, "r");
    if (!pf)
    {
        printf("\nError de Archivo.\n");
        free(vec->vecSecuencia);
        return FILE_ERROR;
    }

    while (i < cant_elem && fscanf(pf, "%d", &vec->vecSecuencia[i]) == 1)
    {
        i++;
    }

    vec->longitud = i;
    fclose(pf);


    return FILE_OK;
}

size_t contarElemSecuencia(char *nombre_archivo)
{
    FILE *pf;
    int num;
    size_t contador = 0;

    pf = fopen(nombre_archivo, "r");

    if (!pf)
    {
        printf("\nError de archivo.\n");
        fclose(pf);
        return FILE_ERROR;
    }

    while (fscanf(pf, "%d", &num) == 1)
    {
        contador++;
    }

    fclose(pf);
    return contador;
}

size_t buscarMaximo(tSecuencia *vec)
{
    int *max = vec->vecSecuencia;

    if (vec->longitud == 0 || vec->vecSecuencia == NULL)
    {
        return 0;
    }

    for (int *i = vec->vecSecuencia + 1; i < vec->vecSecuencia + vec->longitud; i++)
    {
        if (*i > *max)
        {
            max = i;
        }
    }

    return *max;
}

// Pantalla Perdiste

void pantallaPerdiste(tSistemaSDL* sdl,tJugador* jugador)
{
    colorPantalla(sdl,(SDL_Color){180,0,0,255});
    mostrarTextoPerdiste(sdl,jugador);

}

void mostrarTextoPerdiste(tSistemaSDL* sdl,tJugador* jugador)
{
    int x1,x2,x3,y1,y2,y3;
    Uint32 tiempo_actual = SDL_GetTicks();

    SDL_Color color = (SDL_Color){255,255,255,255};
    char texto_jugador[500];
    char texto_parrafo[500];
    char texto_titulo[500];

    strcpy(texto_titulo,"¡PERDISTEE!");
    sprintf(texto_jugador,"Nombre: %s | Score: %d",jugador->nombre,jugador->Score);
    strcpy(texto_parrafo,"Precionar alguna tecla para salir");

    SDL_Surface* superficie_titulo = TTF_RenderText_Blended(sdl->fuente_titulo,texto_titulo,color);
    SDL_Texture* textura_titulo = SDL_CreateTextureFromSurface(sdl->renderer,superficie_titulo);

    SDL_Surface *superficie_jugador = TTF_RenderText_Blended(sdl->fuente2,texto_jugador,color);
    SDL_Surface *superficie_parrafo = TTF_RenderText_Blended(sdl->fuente,texto_parrafo,color);

    SDL_Texture *textura_jugador = SDL_CreateTextureFromSurface(sdl->renderer,superficie_jugador);
    SDL_Texture *textura_parrafo = SDL_CreateTextureFromSurface(sdl->renderer,superficie_parrafo);

    x1 = (ANCHO - superficie_jugador->w) / 2;
    x2 = (ANCHO - superficie_parrafo->w) / 2;

    y1 = 350;
    y2 = 600;

    x3 = (ANCHO - superficie_titulo->w) / 2 + (int)(20 * sin(tiempo_actual/300.0f));
    y3 = 60 + (int)(10 * fabs(sin(tiempo_actual/200.0f)));


    SDL_Rect rect_jugador = {x1, y1, superficie_jugador->w, superficie_jugador->h};
    SDL_Rect rect_parrafo = {x2, y2, superficie_parrafo->w, superficie_parrafo->h};
    SDL_Rect rectangulo = {x3,y3,superficie_titulo->w,superficie_titulo->h};

    SDL_RenderCopy(sdl->renderer,textura_titulo,NULL,&rectangulo);
    SDL_RenderCopy(sdl->renderer,textura_jugador,NULL,&rect_jugador);
    SDL_RenderCopy(sdl->renderer,textura_parrafo,NULL,&rect_parrafo);

    SDL_FreeSurface(superficie_jugador);
    SDL_FreeSurface(superficie_parrafo);
    SDL_FreeSurface(superficie_titulo);
    SDL_DestroyTexture(textura_titulo);
    SDL_DestroyTexture(textura_jugador);
    SDL_DestroyTexture(textura_parrafo);
}
unsigned int controlEventosPantallaPerdiste(SDL_Event* evento, unsigned int estado_actual)
{
    unsigned int estado = estado_actual;

     while (SDL_PollEvent(evento))
    {
        switch (evento->type)
        {
        case SDL_MOUSEBUTTONDOWN:
            if (evento->button.button == SDL_BUTTON_LEFT)
            {
                printf("Hiciste clic izquierdo en %d - %d\n", evento->button.x, evento->button.y);
                printf("\n");
                estado = JUGAR;
            }
            if(evento->button.button == SDL_BUTTON_RIGHT)
            {
                estado = SCHONBERG;
            }
            break;

        case SDL_QUIT:
            estado = SALIR;
            printf("\nEstado actual: Menu Saliendo\n");
            break;
        }
    }

    return estado;
}
