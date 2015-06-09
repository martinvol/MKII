
#include "Arrojable.hpp"
#include <iostream>
#include <cmath>

using namespace std;

Arrojable::Arrojable(bool derecha, SDL_Renderer* ren){
	this->ren = ren;
	this->imagen = imagen;
	this->espejado = derecha;
	this->vel_horizontal = 0;
	this->vel_vertical = 0;
	this->coord = NULL;
	this->alto = 0;
	this->ancho = 0;
	this->rectangulo = NULL;
	//this->imagen = IMG_LoadTexture(this->ren, "resources/arma.png");
	this->imagen = IMG_LoadTexture(this->ren, "resources/riquelme.png");
	this->imagen_congelar = IMG_LoadTexture(this->ren, "resources/riquelme.png");
}


void Arrojable::setCoordenadas(CoordenadaLogica *coord, int alto, int ancho){
	// recibe una coordenada lógica y la convierte a física
	this->coord = coord;
	this->alto_original = alto;
	this->alto = ancho/5.;
	this->ancho = alto/5.;
	this->rectangulo = new Rectangulo(0, 0, 100, 100, true, false);
	this->rectangulo->generar_rectanguloSDL(0, 0, 0, 0, this->ren, false);
}

void Arrojable::tirar(float velocidad_arma){
	this->vel_horizontal = velocidad_arma;
	if (this->espejado){
		this->vel_horizontal *= -1;
	}
	this->vel_vertical = 0;

}

void Arrojable::tirarDiagonal(bool arriba){
	float velocidad = abs(vel_horizontal);
	if (arriba){
		this->vel_vertical = velocidad;
	} else{ // abajo
		this->vel_vertical = velocidad*-1.;
	}
}

bool Arrojable::salioDeLaPantalla(ConversorDeCoordenadas *conv){
	// quizás haya que ver que hacemos si está espejada

	//cout << "ancho lógico" << tamanio_escenario << endl;
	//return false;
	CoordenadaFisica* coord1_fis = conv->aFisica(this->coord);
	bool out = coord1_fis->x_fisico >= conv->ancho_fisico || coord1_fis->x_fisico <= 0 || this->pego;
	out = out || coord1_fis->y_fisico <= 0 || coord1_fis->y_fisico >= conv->alto_fisico;
	delete coord1_fis;
	return out;


}

void Arrojable::dibujar(ConversorDeCoordenadas *conv){

	CoordenadaFisica* coord1_fis = conv->aFisica(this->coord);

	SDL_Rect destino;
	destino.x = coord1_fis->x_fisico ;
	destino.y = coord1_fis->y_fisico + this->alto_original*1./3.;

	destino.w = this->ancho;//ancho_fisico;
	destino.h = this->alto;




	SDL_Point point = {destino.w/2, destino.h};

	int rotar = 0;
	int multiplicar = 1;

	if (this->espejado){
		multiplicar = -1;
	}

	if (vel_vertical > 0){
		rotar = -45*multiplicar;
	} else if (vel_vertical < 0){
		rotar = 45*multiplicar;
	}
	
	if (this->espejado){
		destino.x -= 2*ancho;
		SDL_RenderCopyEx(this->ren, this->imagen, NULL, &destino,rotar,&point,SDL_FLIP_HORIZONTAL);
		this->rectangulo->generar_rectanguloSDL(destino.x + this->vel_horizontal, destino.y - this->vel_vertical, destino.w + abs(this->vel_horizontal), destino.h, this->ren, false);

		/* code */
	} else {
		destino.x += ancho;
		this->rectangulo->generar_rectanguloSDL(destino.x + this->vel_horizontal, destino.y - this->vel_vertical, destino.w + abs(this->vel_horizontal), destino.h, this->ren, false);
		SDL_RenderCopyEx(this->ren, this->imagen, NULL, &destino,rotar,&point,SDL_FLIP_NONE);
	}
	

    delete coord1_fis;

    this->coord->x += this->vel_horizontal;
    this->coord->y += this->vel_vertical;
}

Arrojable::~Arrojable(){
	delete this->coord;
	delete this->rectangulo;
	SDL_DestroyTexture(this->imagen);
	SDL_DestroyTexture(this->imagen_congelar);
}
