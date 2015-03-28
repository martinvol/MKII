#ifndef BARRA_H
#define BARRA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_image.h>
#include <string>

class barraDeVida {
        string path;
        string nombreDelJugador;
        unsigned char cantidadDeVida;
    public:
        barraDeVida(unsigned char cantidadDeVida, string path, string nombreDelJugador);
        restarVida(unsigned char unaCantidad);
}

#endif
