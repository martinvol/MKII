#include "Rectangulo.hpp"
#define MOSTRAR_RECTANGULOS false

using namespace std;

Rectangulo::Rectangulo(float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo){
	this->empieza_alto = empieza_alto/100.;
	this->empieza_ancho = empieza_ancho/100.;
	this->porcentaje_alto = porcentaje_alto/100.;
	this->porcentaje_ancho = porcentaje_ancho/100.;
	this->ataque = ataque;
	this->bloqueo = bloqueo;

}

void Rectangulo::generar_rectanguloSDL(float x_fisico, float y_fisico, float ancho_fisico, float alto_fisico, SDL_Renderer* renderer, bool espejado){
	if (sdl_rec != NULL){
		delete sdl_rec;
	}

	if (ancho_fisico < 5){
		ancho_fisico = 5;
	}

	sdl_rec = new SDL_Rect();

	float empieza_ancho_espejado = empieza_ancho;
	if (espejado){
		empieza_ancho_espejado = 1 - empieza_ancho - porcentaje_ancho;
	}

	sdl_rec->x = x_fisico + ancho_fisico*empieza_ancho_espejado;
	sdl_rec->y = y_fisico + alto_fisico*empieza_alto;
	sdl_rec->w = ancho_fisico*this->porcentaje_ancho;
	sdl_rec->h = alto_fisico*this->porcentaje_alto;

	
	int r=0, g=0, b=0;
	if (this->ataque){
		r = 0xFF;
	} if (this->bloqueo) {
		b = 0xFF;
	} if (!this->bloqueo && !this->ataque){
		g = 0xFF;
	}

	
	//cout << this->ataque << endl;

	if (MOSTRAR_RECTANGULOS){
		SDL_SetRenderDrawColor(renderer, r, g, b, 0xA0);
		SDL_RenderFillRect(renderer, sdl_rec);
	}
}

Rectangulo::~Rectangulo(){
	delete this->sdl_rec;
}