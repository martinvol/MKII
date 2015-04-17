#ifndef TIMER_H
#define TIMER_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "parser.h"
#include "ConversorDeCoordenadas.h"
#include "logger.h"

class Timer {
    SDL_Texture* numeritos;
    unsigned int divisor, actualD, actualU;
    ConversorDeCoordenadas* conv;
    Logger* log;
    SDL_Renderer* ren;
        
    public:
        Timer(unsigned int divisor, string pathDeLaImagenDeTiempo, ConversorDeCoordenadas* conv, SDL_Renderer* ren);
        bool Dibujarse();
        void reset();
        bool terminoElTiempo();  
        void avanzarTimer(unsigned int ticks); 
        ~Timer();    
};

#endif

