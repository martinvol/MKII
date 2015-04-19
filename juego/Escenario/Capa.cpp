#include "Capa.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "../Coordenadas/ConversorDeCoordenadas.h"
#include <string>

using namespace std;

Capa::Capa (string ubicacionParam, float anchoLogicoParam,  float x_logicoParam, SDL_Renderer *rendererParam, ConversorDeCoordenadas* conversor, float ancho_logico_escenario){
    this->ren = rendererParam;
    this->ubicacion = ubicacionParam;
    this->anchoLogico = anchoLogicoParam;
    this->x_logico = x_logicoParam;
    textura = CargarTextura();
     
    if (conversor != NULL){
        this->conversor =  conversor;
        
        int w, h;
        SDL_QueryTexture(this->textura, NULL, NULL, &w, &h);
    
        float a = 0;
        b = this->x_logico;
        float c = ancho_logico_escenario - (this->conversor->ancho_logico);
        float d = w - w*(conversor->ancho_logico/this->anchoLogico);
        //float d = conversor->ancho_logico - this->anchoLogico;
        m = (d -b)/(c-a);
    }

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
	
	float posi_px = (((this->m)*x + (this->b)));
	cout <<"posi_px " << posi_px << "\n"; ///

	posi_px = posi_px*(conversor->factor_ancho) - x;



	SDL_Rect destination_rect;

	destination_rect.x = posi_px + (this->x_logico)*conversor->factor_ancho;
	destination_rect.y = y;
	destination_rect.w = conversor->factor_ancho*this->anchoLogico;
	destination_rect.h = conversor->alto_fisico;
	
	SDL_RenderCopyEx(ren, textura, NULL, &destination_rect, 0.0, NULL, SDL_FLIP_NONE);
	//SDL_RenderCopy(ren, textura, NULL, &destination_rect);


}

void Capa::DibujarseAnchoReal2(int x, int y, ConversorDeCoordenadas* conversor){
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
void Capa::Dibujarse2(int x, int y, ConversorDeCoordenadas* conversor){
	int ancho, alto;
	SDL_QueryTexture(this->textura, NULL, NULL, &ancho, &alto);
    SDL_Rect loQueSeCorta = {x, y, ancho*0.5, alto};


	SDL_RenderCopy(ren, textura, &loQueSeCorta, NULL);//;, 0.0, NULL, SDL_FLIP_NONE);
}

//----------------------------------------------------------------
Capa::~Capa(){
    // Martin ver si imprimo algo en un logger.
    SDL_DestroyTexture(this->textura);
}

