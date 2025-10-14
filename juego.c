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
        boton_simon[i].rectangulo = (SDL_Rect){
            pos_activas[i].x, pos_activas[i].y, LARGO_SIMON, ANCHO_SIMON};
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

    FILE *pf = fopen(nombre_archivo, "r");
    if (!pf)
    {
        printf("\nError de Archivo.\n");
        free(vec->vecSecuencia);
        return FILE_ERROR;
    }

    int i = 0;
    int num_leido;

    while (i < cant_elem && fscanf(pf, "%d", &num_leido) == 1)
    {
        if (num_leido < 0 || num_leido > 7)
        {
            printf("Error de formato.\n");
            fclose(pf);
            free(vec->vecSecuencia);
            vec->vecSecuencia = NULL;
            return FORMATO_ERROR;
        }

        vec->vecSecuencia[i] = num_leido;
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

void pantalla_juego(tSistemaSDL *sdl, tJugador *jugador, bool modo, SDL_Color color, tConfeti *confeti)
{
    colorpantalla_juego(sdl, color);
    actualizarConfeti(confeti);
    dibujarConfeti(sdl, confeti);
    mostrarTexto_juego(sdl, jugador, modo);
}

void mostrarTexto_juego(tSistemaSDL *sdl, tJugador *jugador, bool modo)
{
    int x1, x2, x3, y1, y2, y3, x4, y4;

    SDL_Color color = (SDL_Color){255, 255, 255, 255};
    char texto_jugador[500];
    char texto_parrafo[500];
    char texto_titulo[500];
    char texto_parrafo2[500];

    if (modo == false)
    {
        strcpy(texto_titulo, "�PERDISTEE!");
    }
    else
    {
        strcpy(texto_titulo, "�GANASTE!");
    }
    sprintf(texto_jugador, "Nombre: %s | Score: %d", jugador->nombre, jugador->Score);
    strcpy(texto_parrafo, "Clic izquiero para volver al menu de juego");
    strcpy(texto_parrafo2, "Clic derecho para volver a jugar");

    SDL_Surface *superficie_parrafo2 = TTF_RenderText_Blended(sdl->fuente, texto_parrafo2, color);

    SDL_Surface *superficie_titulo = TTF_RenderText_Blended(sdl->fuente_titulo, texto_titulo, color);
    SDL_Texture *textura_titulo = SDL_CreateTextureFromSurface(sdl->renderer, superficie_titulo);

    SDL_Surface *superficie_jugador = TTF_RenderText_Blended(sdl->fuente2, texto_jugador, color);
    SDL_Surface *superficie_parrafo = TTF_RenderText_Blended(sdl->fuente, texto_parrafo, color);

    SDL_Texture *textura_parrafo2 = SDL_CreateTextureFromSurface(sdl->renderer, superficie_parrafo2);
    SDL_Texture *textura_jugador = SDL_CreateTextureFromSurface(sdl->renderer, superficie_jugador);
    SDL_Texture *textura_parrafo = SDL_CreateTextureFromSurface(sdl->renderer, superficie_parrafo);

    x1 = (ANCHO - superficie_jugador->w) / 2;
    x2 = (ANCHO - superficie_parrafo->w) / 2;

    y1 = 350;
    y2 = 600;

    x3 = (ANCHO - superficie_titulo->w) / 2;
    y3 = 60;

    x4 = (ANCHO - superficie_parrafo2->w) / 2;
    y4 = 700;

    SDL_Rect rect_jugador = {x1, y1, superficie_jugador->w, superficie_jugador->h};
    SDL_Rect rect_parrafo = {x2, y2, superficie_parrafo->w, superficie_parrafo->h};
    SDL_Rect rectangulo = {x3, y3, superficie_titulo->w, superficie_titulo->h};
    SDL_Rect rec_parrafo2 = {x4, y4, superficie_parrafo2->w, superficie_parrafo2->h};

    SDL_RenderCopy(sdl->renderer, textura_parrafo2, NULL, &rec_parrafo2);
    SDL_RenderCopy(sdl->renderer, textura_titulo, NULL, &rectangulo);
    SDL_RenderCopy(sdl->renderer, textura_jugador, NULL, &rect_jugador);
    SDL_RenderCopy(sdl->renderer, textura_parrafo, NULL, &rect_parrafo);

    SDL_FreeSurface(superficie_parrafo2);
    SDL_FreeSurface(superficie_jugador);
    SDL_FreeSurface(superficie_parrafo);
    SDL_FreeSurface(superficie_titulo);

    SDL_DestroyTexture(textura_parrafo2);
    SDL_DestroyTexture(textura_titulo);
    SDL_DestroyTexture(textura_jugador);
    SDL_DestroyTexture(textura_parrafo);
}
unsigned int controlEventosPantalla_juego(SDL_Event *evento, unsigned int estado_actual, bool modo)
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
                estado = MENU;
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

void colorpantalla_juego(tSistemaSDL *sdl, SDL_Color color)
{
    SDL_SetRenderDrawBlendMode(sdl->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(sdl->renderer, color.r, color.g, color.b, color.a);
    SDL_Rect pantalla_completa = {0, 0, ANCHO, LARGO};
    SDL_RenderFillRect(sdl->renderer, &pantalla_completa);
    SDL_SetRenderDrawBlendMode(sdl->renderer, SDL_BLENDMODE_NONE);
    SDL_RenderClear(sdl->renderer);
}

void inicializarConfeti(tConfeti *confeti)
{

    SDL_Color colores[] = {
        {255, 50, 50, 255},  // Rojo
        {50, 255, 50, 255},  // Verde
        {50, 50, 255, 255},  // Azul
        {255, 255, 50, 255}, // Amarillo
        {255, 50, 255, 255}, // Rosa
        {50, 255, 255, 255}  // Cyan
    };

    for (int i = 0; i < MAX_CONFETI; i++)
    {
        confeti[i].tam = rand() % 8 + 4;
        confeti[i].pos.x = rand() % 740;
        confeti[i].pos.y = -(rand() % 200);
        confeti[i].pos.w = confeti[i].tam;
        confeti[i].pos.h = confeti[i].tam;

        confeti[i].color = colores[rand() % 6];

        confeti[i].velocidad_y = rand() % 5 + 2.0f;
        confeti[i].velocidad_x = (rand() % 10 - 5) / 2.0f;
        confeti[i].gravedad = 0.1f;
        confeti[i].rotacion = rand() % 360;
    }
}

void actualizarConfeti(tConfeti *confeti)
{
    for (int i = 0; i < MAX_CONFETI; i++)
    {
        confeti[i].velocidad_y += confeti[i].gravedad;

        confeti[i].pos.y += confeti[i].velocidad_y;
        confeti[i].pos.x += confeti[i].velocidad_x;

        confeti[i].rotacion += confeti[i].velocidad_x;

        if (confeti[i].pos.x < 0 || confeti[i].pos.x > 740 - confeti[i].tam)
        {
            confeti[i].velocidad_x *= -0.8f;
        }

        if (confeti[i].pos.y > 740)
        {
            confeti[i].pos.y = -confeti[i].tam;
            confeti[i].pos.x = rand() % 740;
            confeti[i].velocidad_y = rand() % 3 + 1.0f;
        }
    }
}

void dibujarConfeti(tSistemaSDL *sdl, tConfeti *confeti)
{
    for (int i = 0; i < MAX_CONFETI; i++)
    {
        SDL_SetRenderDrawColor(sdl->renderer,
                               confeti[i].color.r,
                               confeti[i].color.g,
                               confeti[i].color.b,
                               confeti[i].color.a);

        // Dibujar como peque�o rect�ngulo (podr�as hacer formas m�s interesantes)
        SDL_RenderFillRect(sdl->renderer, &confeti[i].pos);

        // Opcional: dibujar borde
        SDL_SetRenderDrawColor(sdl->renderer, 255, 255, 255, 128);
        SDL_RenderDrawRect(sdl->renderer, &confeti[i].pos);
    }
}

void inicializarSecuenciaDesafio(tSecuencia* secuencia)
{
    secuencia->longitud = 1;
    secuencia->indice = 0;
    secuencia->primera_vez = false;
    secuencia->vecSecuencia = NULL;
    secuencia->vecSecuencia = malloc(sizeof(int)* secuencia->longitud);
    if (!secuencia->vecSecuencia)
    {
        printf("Error de memoria.\n");
        secuencia->longitud = 0;
        return;
    }
}


void agregarDesafioSecuencia(tSecuencia *secuencia, unsigned int valor)
{
    int *aux = realloc(secuencia->vecSecuencia, sizeof(int) * secuencia->longitud);
    if (!aux)
    {
        printf("Error de memoria.\n");
        return;
    }

    secuencia->vecSecuencia = aux;

    secuencia->vecSecuencia[secuencia->longitud] = valor;
    secuencia->longitud++;
}

void agregarSecuenciaArchivo(tSecuencia *secuencia, char* archivo)
{
    FILE *pf = fopen(archivo,"w");

    if(!pf)
    {
        printf("Error al abrir el archivo\n");
        return;
    }

    for (int* i = secuencia->vecSecuencia; i < secuencia->vecSecuencia + secuencia->longitud; i++)
    {
        fprintf(pf, "%d ", *i);
    }

    fclose(pf);
}
