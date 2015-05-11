
#include "Arrojable.hpp"
#include <iostream>

using namespace std;

Arrojable::Arrojable(SDL_Texture *imagen, bool derecha, SDL_Renderer* ren){
	cout << "el personaje sabe que tiene que arrojar el arma2" << endl; 
	this->ren = ren;
	this->imagen = imagen;
	cout << "el personaje sabe que tiene que arrojar el arma3" << endl; 
}


void Arrojable::setCoordenadas(CoordenadaLogica *coord, float alto, float ancho){
	// recibe una coordenada lógica y la convierte a física
	this->coord = coord;
	this->alto = alto;
	this->ancho = ancho;
	this->rectangulo = new Rectangulo(0, 0, 100, 100, true, false);
	this->rectangulo->generar_rectanguloSDL(0, 0, 0, 0, this->ren, false);
}

void Arrojable::tirar(){
	this->vel_horizontal = 2;
	this->vel_vertical = 0;

}

void Arrojable::tirarDiagonal(bool arriba){
}

bool Arrojable::salioDeLaPantalla(float tamanio_escenario){
	// quizás haya que ver que hacemos si está espejada

	//cout << "ancho lógico" << tamanio_escenario << endl;
	//return false;
	return this->coord->x >= tamanio_escenario || this->coord->x <= 0 || this->pego;

}

void Arrojable::dibujar(ConversorDeCoordenadas *conv){
	// cout << "dibujo"<< endl;

	CoordenadaFisica* coord1_fis = conv->aFisica(this->coord);

	SDL_Rect destino;
	destino.x = coord1_fis->x_fisico;
	destino.y = coord1_fis->y_fisico;

	/*destino.x = 200;
	destino.y = 200;*/

	//cout << destino.x << endl;
	//cout << destino.y << endl;

	destino.w = 125;//ancho_fisico;
	destino.h = 125; //

	this->rectangulo->generar_rectanguloSDL(destino.x, destino.y, destino.w, destino.h, this->ren, false);

	SDL_Point point = {destino.w/2, destino.h};
	
	SDL_RenderCopyEx(this->ren, this->imagen, NULL, &destino,0,&point,SDL_FLIP_NONE);

    delete coord1_fis;

    this->coord->x +=10;
}