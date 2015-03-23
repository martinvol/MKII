#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#ifndef CAPA_H
#define CAPA_H

class Capa {
    //string pathDeCapa;
    //SDL_Texture* capa;
    unsigned float ancho;
    Coordenada coord;
    public:
        int z-index;
        //string obtenerPath();
        //SDL_Texture* obtenerTexturaDeCapa();
        Coordenada obtenerCoordenada();
}

#endif
