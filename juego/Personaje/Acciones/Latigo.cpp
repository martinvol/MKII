#include "Latigo.hpp"
#include <cmath>
#include <iostream>

using namespace std;

Latigo::Latigo(SDL_Renderer* ren, int alto, int ancho){
	this->ren = ren;
	this->imagen = imagen;
	// this->espejado = derecha;
	this->vel_horizontal = 0;
	this->alto = alto;
	this->ancho = ancho;
	this->nuevo_ancho = 0;
	this->rectangulo = NULL;
	this->imagen = IMG_LoadTexture(this->ren, "resources/latigo.png");// this->imagen = IMG_LoadTexture(this->ren, "resources/arma.png");
}

Latigo::~Latigo(){
	SDL_DestroyTexture(this->imagen);
	delete coord;
}

void Latigo::tirar(CoordenadaLogica *coord, float velocidad_arma, float alto_personaje, float ancho_personaje){
	ancho_original = ancho_personaje;
	alto_original = alto_personaje;
	this->coord = coord;
	vel_horizontal = velocidad_arma;
	this->rectangulo = new Rectangulo(0, 0, 100, 100, false, false);
	this->rectangulo->generar_rectanguloSDL(0, 0, 0, 0, this->ren, false);
	volver = false;
}

void Latigo::dibujar(ConversorDeCoordenadas *conv){

	if (this->coord){
		CoordenadaFisica* coord1_fis = conv->aFisica(this->coord);

		SDL_Rect destino;
		destino.x = coord1_fis->x_fisico ;
		destino.y = coord1_fis->y_fisico + this->alto_original*1./3.;
		/*
		destino.w = this->ancho;
		destino.h = this->alto;*/

		//destino.w = 100;
		destino.h = 100;

		destino.w = nuevo_ancho;
		//destino.h = alto;

		SDL_Point point = {destino.w/2, destino.h};

		int rotar = 0;
		int multiplicar = 1;
		
		if (this->espejado){
			// mirar derecha
			puts("mirar derecha");
			destino.x += ancho_original;
			this->rectangulo->generar_rectanguloSDL(destino.x, destino.y, destino.w + this->vel_horizontal, destino.h, this->ren, false);
			SDL_RenderCopyEx(this->ren, this->imagen, NULL, &destino,rotar,&point,SDL_FLIP_HORIZONTAL);
		} else {
			puts("mirar izquierda");
			destino.x -= ancho_original;
			destino.x -= destino.w;
			this->rectangulo->generar_rectanguloSDL(destino.x, destino.y, destino.w + this->vel_horizontal, destino.h, this->ren, false);
			SDL_RenderCopyEx(this->ren, this->imagen, NULL, &destino,rotar,&point,SDL_FLIP_NONE);
		}
		

	    delete coord1_fis;

	    if (!volver){
	    	this->nuevo_ancho += this->vel_horizontal*4;
	    	if (nuevo_ancho>ancho){
	    		volver = true;
	    	}
	    } else {
	    	this->nuevo_ancho -= this->vel_horizontal*4;
	    	if (this->nuevo_ancho<0){
	    		delete coord;
	    		coord = NULL;
	    		pego = false;
	    	}
	    }
	    
	    cout << nuevo_ancho<< endl;
	}

}

CoordenadaLogica* Latigo::obtenerPunta(ConversorDeCoordenadas *conv){
	int x;
	CoordenadaFisica* coord1_fis = conv->aFisica(this->coord);

	if (this->espejado){
		// mirar derecha
		// destino.x += ancho_original;
		x = coord1_fis->x_fisico +ancho_original + nuevo_ancho;
	} else {
		// mirar izquierda
		/*destino.x -= ancho_original;
		destino.x -= destino.w;*/
		x = ancho_original - ancho_original - nuevo_ancho;
	}
	CoordenadaFisica* temp = new CoordenadaFisica(x ,0);

	CoordenadaLogica* out = conv->aLogica(temp);
	
	delete temp;
	delete coord1_fis;
	return out;
}