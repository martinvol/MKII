#ifndef TIMER_H
#define TIMER_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Parser.hpp"
#include "ConversorDeCoordenadas.hpp"


class Timer {
    SDL_Texture* numeritos;
    public:
        Timer(unsigned int ticks, unsigned int divisor, string pathDeLaImagenDeTiempo, ConversorDeCoordenadas* conv);
        void reset();
        bool terminoElTiempo();  
        bool dibujarse(int xFisico, int yFisico, float altoLogico, float anchoLogico);      
}

#endif

