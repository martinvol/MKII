#include "Capa.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "ConversorDeCoordenadas.h"

using namespace std;

Capa::Capa (string ubicacionParam, float anchoLogicoParam,  float x_logicoParam, SDL_Renderer *rendererParam){
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

void Capa::DibujarseAnchoReal(int x, int y, ConversorDeCoordenadas* conversor){
	//Dibujarse(x,y, conversor->alto_fisico, conversor->factor_ancho*this->anchoLogico);
	

	SDL_Rect destination_rect;

	destination_rect.x = x;
	destination_rect.y = y;
	destination_rect.w = conversor->factor_ancho*this->anchoLogico;
	destination_rect.h = conversor->alto_fisico;

	SDL_RenderCopyEx(ren, textura, NULL, &destination_rect, 0.0, NULL, SDL_FLIP_NONE);
}

//----------------------------------------------------------------
void Capa::Dibujarse(int x, int y, int alto, int ancho){
	//Rectangulo destino
	SDL_Rect destino;
	destino.h = alto;
	destino.w = ancho;
	destino.x = x;
	destino.y = y;
	// printf("%d\n", destino.w);
	SDL_RenderCopy(this->ren, this->textura, NULL, &destino);
}

//----------------------------------------------------------------
void Capa::Dibujarse2(int x, int y){
	int ancho, alto;
	SDL_QueryTexture(this->textura, NULL, NULL, &ancho, &alto);
    SDL_Rect loQueSeCorta = {x, y, ancho*0.5, alto};


	SDL_RenderCopyEx(ren, textura, &loQueSeCorta, NULL, 0.0, NULL, SDL_FLIP_NONE);
}

//----------------------------------------------------------------
Capa::~Capa(){
    // Martin ver si imprimo algo en un logger.
    SDL_DestroyTexture(this->textura);
}

