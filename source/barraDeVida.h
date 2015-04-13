#ifndef BARRA_H
#define BARRA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL2_image.h>
#include <string>

class BarraDeVida {
        string path;
        string nombreDelJugador;
        int cantidadDeVida;
    public:
        BarraDeVida(int cantidadDeVida, string path, string nombreDelJugador);
        void restarVida(int unaCantidad);
        //SDL_Texture* dibujar();
}

#endif
