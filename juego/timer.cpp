#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "parser.h"
#include "ConversorDeCoordenadas.h"
#include "timer.hpp"

using namespace std;
#define IMG_DEFAULT "resources/miscelaneo/06.gif"

Timer::Timer(unsigned int divisor, string pathDeLaImagenDeTiempo, ConversorDeCoordenadas* conv, SDL_Renderer* ren) {
    this->log = Logger::instance();
    this->numeritos = IMG_LoadTexture(ren, pathDeLaImagenDeTiempo.c_str());
    if (!this->numeritos) log->log_error("No se pudo cargar la imagen correspondiente al timer");
    // Falta cargar una por default
    this->divisor = divisor;
    this->conv = conv;
    this->ren = ren;
    this->actualD = 0;
    this->actualU = 0;
}

void Timer::reset() { 
    this->actualD = 0;
    this->actualU = 0;
}

bool Timer::terminoElTiempo() {
    return (this->actualD == this->actualU == 0);
}
  
bool Timer::dibujarse(int xFisico, int yFisico, float altoLogico, float anchoLogico) {

    int w, h;
    SDL_QueryTexture(this->numeritos, NULL, NULL, &w, &h);
    // int(this->conv->alto_fisico* 0.1f)
    SDL_Rect srcrect = { this->actualU * (w / 10) - 3,0, (w/10), h };
    SDL_Rect srcrect2 = { this->actualD * (w / 10) - 3, 0, (w/10), h} ;
    SDL_Rect dstrect2 = { xFisico - (w/10) - 3, 100, anchoLogico, altoLogico };
    SDL_Rect dstrect = { xFisico, yFisico, anchoLogico, altoLogico };
    SDL_RenderCopy(this->ren, this->numeritos, &srcrect, &dstrect);
    SDL_RenderCopy(this->ren, this->numeritos, &srcrect2, &dstrect2);
    return true;
}   

void Timer::avanzarTimer(unsigned int ticks) {
    this->actualU = (ticks / this->divisor) % 10;
    this->actualD = (ticks / (this->divisor*10)) % 10;    
}

Timer::~Timer() {
    SDL_DestroyTexture(this->numeritos);
}




/*//Comienza en el 10% de la pantalla
    this->y_ini = 
    //Usa 10% del alto.
    this->y_fin = y_ini+ int(altoPantalla * 0.07f);*/

