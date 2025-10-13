#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

//Datos hitbox rectangulo
#define LARGO_RECT 200
#define ANCHO_RECT 50
#define POSICION_X 275
#define POSICION_Y 300
#define PADDING 80

//Constantes para estados
#define MENU 10
#define JUGAR 20
#define OPCIONES 30
#define ESTADISTICA 40
#define SALIR 50
#define SCHONBERG 60
#define MOZART 70
#define PERDISTE 80
#define GANASTE 90
#define SIGUIENTE 120
#define ANTERIOR 121
#define EST_SCHO 123
#define EST_MO 124

#define TOTAL_ESTADOS 8

//Constantes para cantidad de botones
#define CANTIDAD_BOTON_MENU 4
#define CANTIDAD_BOTON_JUGAR 3
#define CANTIDAD_BOTON_OPCIONES 1
#define CANTIDAD_BOTON_ESTADISTICA 1

//Constantes tamaño de pantalla
#define TAM_PIXEL 10
#define PIXELES_X_LADO 12
#define TAM_GRILLA 6
#define PX_PADDING 4
#define ANCHO (TAM_GRILLA * TAM_PIXEL * PIXELES_X_LADO + TAM_GRILLA * PX_PADDING)
#define LARGO (TAM_GRILLA * TAM_PIXEL * PIXELES_X_LADO + TAM_GRILLA * PX_PADDING)

//Constantes de limites
#define MAX_LETRAS 20
#define MAX_BOTONES 3

//crab
#define MAX_CRAB 10
#define T 0
#define R 1
#define BR 2 // Borde

//valor boton simon
#define BOTON_1 0
#define BOTON_2 1
#define BOTON_3 2
#define BOTON_4 3
#define BOTON_5 4
#define BOTON_6 5
#define BOTON_7 6
#define BOTON_8 7
#define ANCHO_SIMON 180
#define LARGO_SIMON 180


#endif // CONSTANTES_H_INCLUDED
