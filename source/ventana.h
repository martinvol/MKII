#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<string>

#ifndef VENTANA_H
#define VENTANA_H

class Ventana {
        //SDL_Texture *colorDeFondo;
        //SDL_Window* ventana;
        unsigned int anchoPx, altoPx;
        unsigned int ancho, alto;
        Coordenada coordEsquinaInferiorIzq;
        
    public:
        Ventana(unsigned int alto, unsigned int ancho, unsigned int altoPx, unsigned int anchoPx, string titulo);
        Coordenada obtenerCoordEsquinaInferiorIzq(); // Deberia poder moverse
        ~Ventana;
};

#endif 
