
#include "Arrojable.hpp"

using namespace std;

Arrojable::Arrojable(string ruta, bool derecha, SDL_Renderer* ren){
	this->imagen = IMG_LoadTexture (this->ren, ruta.c_str());
	this->ren = ren;
}


void Arrojable::set_coordenadas(float x, float y, float alto, float ancho){
	// recibe una coordenada lógica y la convierte a física
	this->x = x;
	this->y = y;	
	this->alto = alto;
	this->ancho = ancho;
}

void Arrojable::tirar(){
	this->vel_horizontal = 2;
	this->vel_vertical = 0;

}

void Arrojable::dibujar(){
	/*SDL_Rect destino;
	destino.x = coord1_fis->x_fisico;
	destino.y = coord2_fis->y_fisico;
	destino.w = (_w)*this->conversor->factor_ancho;//ancho_fisico;
	destino.h = alto_fisico; //
	SDL_RenderCopyEx(this->renderer, this->imagenActual, NULL, &destino,0,&point,SDL_FLIP_HORIZONTAL);*/

}