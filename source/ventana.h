#ifndef VENTANA_H
#define VENTANA_H

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<string>

class Ventana {
        //SDL_Texture *colorDeFondo;
        //SDL_Window* ventana;
        unsigned int anchoPx, altoPx;
        unsigned float ancho, alto; //Dani: Esto creo que no es necesario.
        CoordenadaLogica coordEsquinaInferiorIzq;
        
    public:
        Ventana(unsigned float alto, unsigned float ancho, unsigned int altoPx, unsigned int anchoPx, string titulo);
        CoordenadaLogica obtenerCoordEsquinaInferiorIzq(); // Deberia poder moverse
        ~Ventana;
};

#endif 
