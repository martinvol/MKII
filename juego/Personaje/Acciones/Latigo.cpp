#include "Latigo.hpp"
#include <cmath>


Latigo::Latigo(SDL_Renderer* ren, int alto, int ancho){
	this->ren = ren;
	this->imagen = imagen;
	// this->espejado = derecha;
	this->vel_horizontal = 0;
	this->vel_vertical = 0;
	this->alto = alto;
	this->ancho = ancho;
	this->rectangulo = NULL;
	this->imagen = IMG_LoadTexture(this->ren, "resources/latigo.png");// this->imagen = IMG_LoadTexture(this->ren, "resources/arma.png");
}

Latigo::~Latigo(){
	SDL_DestroyTexture(this->imagen);
}

void Latigo::tirar(CoordenadaLogica *coord, float velocidad_arma){
	this->coord = coord;
	this->vel_vertical = velocidad_arma;
	this->rectangulo = new Rectangulo(0, 0, 100, 100, true, false);
	this->rectangulo->generar_rectanguloSDL(0, 0, 0, 0, this->ren, false);
}

void Latigo::dibujar(ConversorDeCoordenadas *conv){

	if (this->coord){
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

}