#ifndef TIMER_H
#define TIMER_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Parser/Parser.hpp"
#include "../Coordenadas/ConversorDeCoordenadas.hpp"
#include "../Logger/Logger.hpp"

class Timer {
    SDL_Texture* numeritos;
    unsigned int divisor, actualD, actualU;
    ConversorDeCoordenadas* conv;
    Logger* log;
    SDL_Renderer* ren;
    unsigned long long ticks;
    bool freezed;
        
    public:
        Timer(unsigned int divisor, string pathDeLaImagenDeTiempo, ConversorDeCoordenadas* conv, SDL_Renderer* ren);
        bool Dibujarse();
        void reset(unsigned int ticks);
        bool terminoElTiempo();  
        void avanzarTimer(unsigned int ticks); 
        void pausarTimer(unsigned int ticks);
        ~Timer();    
};

#endif

