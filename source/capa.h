#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#ifndef CAPA_H
#define CAPA_H

class Capa {
    //string pathDeCapa;
    //SDL_Texture* capa;
    unsigned float alto, ancho;
    
    Coordenada coord;
    public:
        //string obtenerPath();
        //SDL_Texture* obtenerTexturaDeCapa();
        Capa(unsigned float alto, unsigned float ancho);
        Coordenada obtenerCoordenada();
}

#endif
