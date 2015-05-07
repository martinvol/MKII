#include "Rectangulo.hpp"
#define MOSTRAR_RECTANGULOS true

using namespace std;

Rectangulo::Rectangulo(float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque){
	this->empieza_alto = empieza_alto;
	this->empieza_ancho = empieza_ancho;
	this->porcentaje_alto = porcentaje_alto;
	this->porcentaje_ancho = porcentaje_ancho;
	this->ataque = ataque;

}

void Rectangulo::generar_rectanguloSDL(float x_fisico, float y_fisico, float ancho_fisico, float alto_fisico, SDL_Renderer* renderer){
	if (sdl_rec != NULL){
		delete sdl_rec;
	}
	SDL_Rect fillRect = {x_fisico, y_fisico, ancho_fisico, alto_fisico };
	if (MOSTRAR_RECTANGULOS){
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xA0);
		SDL_RenderFillRect(renderer, &fillRect);
	}
}