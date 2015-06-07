#include "GrillaDeJugadores.hpp"
#include "../LTexture.hpp"

#define PATH1 "SubZero"
#define PATH2 "Ermac"
#define PATH3 "Rain"
#define PATH4 "NoobSaibot"
#define PATH5 "Scorpion"
#define PATH6 "Reptile"
#define PATH7 "Jade"
#define PATH8 "Mileena"
#define PATH9 "Kitana"
#define PATH10 "Smoke_r"
#define PATH11 "Sektor"
#define PATH12 "Smoke"

#define BACKGROUND "resources/orangeportal.png"
#define OPCION "resources/menu/opcion_transparent.png"

#define X_INIT 80
#define Y_INIT 100

#define CANT_ANCHO 4
#define TOTAL_IMAGENES 12

using namespace std;


Grilla::Grilla(SDL_Renderer* renderer) {
	this->ren = renderer;
	this->paths.push_back(PATH1);
	this->paths.push_back(PATH2);
	this->paths.push_back(PATH3);
	this->paths.push_back(PATH4);
	this->paths.push_back(PATH5);
	this->paths.push_back(PATH6);
	this->paths.push_back(PATH7);
	this->paths.push_back(PATH8);
	this->paths.push_back(PATH9);
	this->paths.push_back(PATH10);
	this->paths.push_back(PATH11);
	this->paths.push_back(PATH12);
	string pathBackground = BACKGROUND;
	this->background = IMG_LoadTexture(this->ren, pathBackground.c_str());
	LTexture aux = LTexture(this->ren);
	aux.loadFromFile(OPCION, 0, 0, 0, false);
	this->seleccion.push_back(aux.mTexture);
	aux.loadFromFile(OPCION, 0, 60, 300, false);
	this->seleccion.push_back(aux.mTexture);
	this->xSeleccion.push_back(X_INIT);
	this->ySeleccion.push_back(Y_INIT);
}

void Grilla::Dibujarse(){ 
	this->cargarTexturas();
	SDL_Rect rect;
	rect.w = this->anchoImagen;
	rect.h = this->altoImagen;
	SDL_RenderCopy(this->ren, this->background, NULL, NULL);
	//this->texs.size();
	for(int i = 0; i < TOTAL_IMAGENES; i++) {
		rect.x = (i % CANT_ANCHO)*this->anchoImagen + X_INIT;
		rect.y = (i / CANT_ANCHO)*this->altoImagen + Y_INIT;
		SDL_RenderCopy(this->ren, this->texs[i], NULL, &rect);
	}
	Uint32 ticks = SDL_GetTicks();
	rect.x = this->xSeleccion[0];
	rect.y = this->ySeleccion[0];
	SDL_RenderCopy(this->ren, this->seleccion[(ticks/100) % 2], NULL, &rect);
}

void Grilla::cargarTexturas() {
	for(int i=0; i < this->paths.size(); i++) {
		string pathCompleto = "resources/jugador/"+this->paths[i]+"/"+this->paths[i]+".gif";
		this->texs.push_back(IMG_LoadTexture(this->ren, pathCompleto.c_str()));
	}
	SDL_QueryTexture(this->texs[0], NULL, NULL, &this->anchoImagen, &this->altoImagen);
	this->anchoImagen *= 1.5;
	this->altoImagen *= 1.5;
}

Grilla::~Grilla() {
	for(int i = 0; i < this->texs.size(); i++) {
		SDL_DestroyTexture(this->texs[i]);
	}
	for(int i = 0; i < this->seleccion.size(); i++) {
		SDL_DestroyTexture(this->seleccion[i]);
	}
	
	SDL_DestroyTexture(this->background);
}

/*********************************************************************
 * ************************  CONTROLLER  *****************************
 * ******************************************************************/

void Grilla::subirOpcion(int jugador) {
	this->ySeleccion[jugador] -= Y_INIT;
	this->ySeleccion[jugador] -= this->altoImagen;
	this->ySeleccion[jugador] = (this->ySeleccion[jugador] + (TOTAL_IMAGENES/CANT_ANCHO)*this->altoImagen) 
								% (TOTAL_IMAGENES/CANT_ANCHO)*this->altoImagen;
	this->ySeleccion[jugador] += Y_INIT;
}
void Grilla::bajarOpcion(int jugador) {
	this->ySeleccion[jugador] -= Y_INIT;
	this->ySeleccion[jugador] += this->altoImagen;
	this->ySeleccion[jugador] = (this->ySeleccion[jugador] + (TOTAL_IMAGENES/CANT_ANCHO)*this->altoImagen) 
								% (TOTAL_IMAGENES/CANT_ANCHO)*this->altoImagen;
	this->ySeleccion[jugador] += Y_INIT;
}
void Grilla::moverDerechaOpcion(int jugador) {
	this->xSeleccion[jugador] -= X_INIT;
	this->xSeleccion[jugador] += this->anchoImagen;
	if (this->xSeleccion[jugador] >= (CANT_ANCHO)*this->anchoImagen) this->xSeleccion[jugador] -= (CANT_ANCHO)*this->anchoImagen;
	this->xSeleccion[jugador] += X_INIT;
}
void Grilla::moverIzquierdaOpcion(int jugador) {
	this->xSeleccion[jugador] -= X_INIT;
	this->xSeleccion[jugador] -= this->anchoImagen;
	if (this->xSeleccion[jugador] < 0) this->xSeleccion[jugador] += (CANT_ANCHO)*this->anchoImagen;
	this->xSeleccion[jugador] += X_INIT;
}
//string Grilla::seleccionarOpcion(int jugador);
