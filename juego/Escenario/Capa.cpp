#include "Capa.hpp"

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "../Coordenadas/ConversorDeCoordenadas.hpp"
#include <string>

using namespace std;


Capa::Capa (string ubicacionParam, float anchoLogicoParam,  float x_logicoParam, SDL_Renderer *rendererParam, float ancho_escenario, float ancho_ventana_logico){
    this->ren = rendererParam;
    this->ubicacion = ubicacionParam;
    this->anchoLogico = anchoLogicoParam;
    this->x_logico = x_logicoParam;
    textura = CargarTextura();
	int w, h;
	SDL_QueryTexture(this->textura, NULL, NULL, &w, &h);

	float a = 0;
	b = this->x_logico;
	float c = ancho_escenario - ancho_ventana_logico;
	float d = w - w*(ancho_ventana_logico/this->anchoLogico);
	//float d = conversor->ancho_logico - this->anchoLogico;
	m = (d -b)/(c-a);
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
void Capa::DibujarseAnchoReal(int x, int y, ConversorDeCoordenadas* conversor){
    // Este metodo va a tratar de dibujar los rectangulos bonitos usando el rect de source
	
	float posi_px = (((this->m)*x + (this->b)));

    SDL_Rect source_rect;
    int w, h;
    SDL_QueryTexture(this->textura, NULL, NULL, &w, &h);
    
    source_rect.x = posi_px;
    source_rect.w = w*(conversor->ancho_logico/this->anchoLogico);
    if (source_rect.x < 0) source_rect.x = 0;
    //else if (source_rect.x >= w - source_rect.w) source_rect.x = w - source_rect.w;
	source_rect.y = 0;
	source_rect.h = h;
	
	SDL_RenderCopy(ren, textura, &source_rect, NULL);
}

//----------------------------------------------------------------
Capa::~Capa(){
    // Martin ver si imprimo algo en un logger.
    SDL_DestroyTexture(this->textura);
}

