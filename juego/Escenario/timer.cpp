#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Parser/parser.h"
#include "../Coordenadas/ConversorDeCoordenadas.h"
#include "timer.hpp"

using namespace std;
#define IMG_DEFAULT "resources/miscelaneo/06.png"

Timer::Timer(unsigned int divisor, string pathDeLaImagenDeTiempo, ConversorDeCoordenadas* conv, SDL_Renderer* ren) {
    this->log = Logger::instance();
    this->numeritos = NULL;
    int intentos = 0;
    while ((!this->numeritos) && (intentos < 10)) {
        this->numeritos = IMG_LoadTexture(ren, pathDeLaImagenDeTiempo.c_str());
        intentos ++;
    }
    if (!this->numeritos) log->log_error("No se pudo cargar la imagen correspondiente al timer");
    this->divisor = divisor;
    this->conv = conv;
    this->ren = ren;
    this->actualD = 8;
    this->actualU = 8;
}

void Timer::reset() { 
    this->actualD = 8;
    this->actualU = 8;
}

bool Timer::terminoElTiempo() {
    return ((this->actualD == 9) && (this->actualU == 9));
}
  
bool Timer::Dibujarse() {

    int w, h;
    float altoLogico = this->conv->alto_fisico * 0.05f;
    float anchoLogico = this->conv->ancho_fisico * 0.028f;
    int xFisico = this->conv->ancho_fisico/2;
    SDL_QueryTexture(this->numeritos, NULL, NULL, &w, &h);
    // int(this->conv->alto_fisico* 0.1f)
    SDL_Rect srcrect = { this->actualU * ((w / 10) + 1) - 3,0, (w/10), h };
    SDL_Rect srcrect2 = { this->actualD *((w / 10) + 1) - 3, 0, (w/10), h} ;
    SDL_Rect dstrect2 = { xFisico - anchoLogico, int(this->conv->alto_fisico* 0.1f), anchoLogico, altoLogico };
    SDL_Rect dstrect = { xFisico, int(this->conv->alto_fisico* 0.1f), anchoLogico, altoLogico };
    SDL_RenderCopy(this->ren, this->numeritos, &srcrect, &dstrect);
    SDL_RenderCopy(this->ren, this->numeritos, &srcrect2, &dstrect2);
    return true;
}   

void Timer::avanzarTimer(unsigned int ticks) {
    if (!this->terminoElTiempo()) {
        this->actualU = 8 - ((ticks / this->divisor)) % 10;
        if (this->actualU > 8) this->actualU = 9;
        this->actualD = 8 - ((ticks / (this->divisor*10))) % 10;
        if (this->actualD > 8) this->actualD = 9; 
    }   
}

Timer::~Timer() {
    SDL_DestroyTexture(this->numeritos);
}



/*//Comienza en el 10% de la pantalla
    this->y_ini = 
    //Usa 10% del alto.
    this->y_fin = y_ini+ int(altoPantalla * 0.07f);*/

