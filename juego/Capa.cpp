#include "Capa.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
using namespace std;

Capa::Capa (string ubicacionParam, double anchoLogicoParam,  double x_logicoParam, SDL_Renderer *rendererParam){
    this->ren = rendererParam;
    this->ubicacion = ubicacionParam;
    this->anchoLogico = anchoLogicoParam;
    this->x_logico = x_logicoParam;
    textura = CargarTextura();
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

//----------------------------------------------------------------
void Capa::Dibujarse(int x, int y){
    int ancho, alto;
	SDL_QueryTexture(this->textura, NULL, NULL, &ancho, &alto);
	Dibujarse(x, y, alto, ancho);
}

//----------------------------------------------------------------
void Capa::Dibujarse(int x, int y, int alto, int ancho){
	//Rectangulo destino
	SDL_Rect destino;
	destino.x = x;
	destino.y = y;
	destino.w = ancho;
	destino.h = alto;
	SDL_RenderCopy(this->ren, this->textura, NULL, &destino);
}

//----------------------------------------------------------------
Capa::~Capa(){
    // Martin ver si imprimo algo en un logger.
    SDL_DestroyTexture(this->textura);
}

