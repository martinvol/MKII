#include "Latigo.hpp"


Latigo::Latigo(bool derecha, SDL_Renderer* ren, int alto, int ancho){
	this->ren = ren;
	this->imagen = imagen;
	this->espejado = derecha;
	this->vel_horizontal = 0;
	this->vel_vertical = 0;
	this->coord = NULL;
	this->alto = alto;
	this->ancho = ancho;
	this->rectangulo = NULL;
	this->imagen = IMG_LoadTexture(this->ren, "resources/latigo.png");// this->imagen = IMG_LoadTexture(this->ren, "resources/arma.png");
}