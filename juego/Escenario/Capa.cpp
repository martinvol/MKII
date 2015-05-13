#include "Capa.hpp"

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "../Coordenadas/ConversorDeCoordenadas.hpp"
#include <string>
#include <math.h>

using namespace std;
#define OFFSET 0.05
#define DIVISORDETIEMPO 100.0

int calcularYSegunTicks(unsigned int ticks, unsigned int t0) {
// Funcion auxiliar
// Dada una cantidad de ticks, permite calcular la posicion en y en la que
// deberia posicionarse la capa. El t0 o t inicial es this->ticks.

    
    float t = (ticks - t0) / DIVISORDETIEMPO;
    if (t0 == 0) return OFFSET/2;
    //if (t > 100 || t < -100) return OFFSET/2;
    return ((-OFFSET/2) * (sin(t*t) / t) + OFFSET/2)*100;
}


Capa::Capa (string ubicacionParam, float anchoLogicoParam,  float x_logicoParam, SDL_Renderer *rendererParam, float ancho_escenario, float ancho_ventana_logico){
    this->ren = rendererParam;
    this->ubicacion = ubicacionParam;
    this->anchoLogico = anchoLogicoParam;
    this->x_logico = x_logicoParam; 
    this->ticks = 0;   
    textura = CargarTextura();
     

    int w, h;
    SDL_QueryTexture(this->textura, NULL, NULL, &w, &h);
    this->y_fisico = (OFFSET*h)*0.5;
    float a = 0;
    b = this->x_logico;
    float c = ancho_escenario - ancho_ventana_logico;
    float d = w - w*(ancho_ventana_logico/this->anchoLogico);
    //float d = conversor->ancho_logico - this->anchoLogico;
    m = (d -b)/(c-a);



	this->ancho_logico_ventana = ancho_ventana_logico;
}

//----------------------------------------------------------------
SDL_Texture* Capa::CargarTextura(){
	SDL_Texture *texturaAux = IMG_LoadTexture(ren, this->ubicacion.c_str());
	if (texturaAux == NULL){
        //Martin
		//ImprimirError(cout, "No se pudo cargar la textura");
	}
	return texturaAux;
}

void Capa::DibujarseAnchoReal(int x, int y){
    // Este metodo va a tratar de dibujar los rectangulos bonitos usando el rect de source
	
	float posi_px = (((this->m)*x + (this->b)));

    SDL_Rect source_rect;
    int w, h;
    SDL_QueryTexture(this->textura, NULL, NULL, &w, &h);
    
    source_rect.x = posi_px;
    source_rect.w = w*(ancho_logico_ventana/this->anchoLogico);
    if (posi_px < 0) source_rect.x = 0;
    //else if (source_rect.x >= w - source_rect.w) source_rect.x = w - source_rect.w;
	//source_rect.y = this->y_fisico;
	source_rect.y = calcularYSegunTicks(SDL_GetTicks(), this->ticks);
	source_rect.h = h - h*OFFSET;
	
	SDL_RenderCopy(ren, textura, &source_rect, NULL);
}

//----------------------------------------------------------------

void Capa::Temblar(unsigned int ticks) {
// Esta funcion hace que haga variar el y_fisico de la capa. Dado
// que necesito una referencia de tiempo, recibe la cantidad de 
// tiempo en ms que pasaron desde que se inicializo SDL.
// y_fisico pertenece a [0, OFFSET]. 
// Los ticks los manejo en una funcion auxiliar y asi calculo 
// el y_fisico correspondiente.
    this->ticks = ticks;
}
//----------------------------------------------------------------

Capa::~Capa(){
    // Martin ver si imprimo algo en un logger.
    SDL_DestroyTexture(this->textura);
}

