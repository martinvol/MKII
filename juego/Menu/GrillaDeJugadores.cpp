#include "GrillaDeJugadores.hpp"

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

#define CANT_ANCHO 6

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
}

void Grilla::Dibujarse(){ 
	this->cargarTexturas();
	SDL_Rect rect;
	rect.w = this->anchoImagen;
	rect.h = this->altoImagen;
	SDL_RenderCopy(this->ren, this->background, NULL, NULL);
	//this->texs.size();
	for(int i = 0; i < 12; i++) {
		rect.x = (i % CANT_ANCHO)*this->anchoImagen + 30;
		rect.y = (i / CANT_ANCHO)*this->altoImagen + 60;
		SDL_RenderCopy(this->ren, this->texs[i], NULL, &rect);
	}
}

void Grilla::cargarTexturas() {
	for(int i=0; i < this->paths.size(); i++) {
		string pathCompleto = "resources/jugador/"+this->paths[i]+"/"+this->paths[i]+".gif";
		this->texs.push_back(IMG_LoadTexture(this->ren, pathCompleto.c_str()));
	}
	SDL_QueryTexture(this->texs[0], NULL, NULL, &this->anchoImagen, &this->altoImagen);
	this->anchoImagen *= 2;
	this->altoImagen *= 2;
}

Grilla::~Grilla() {
	for(int i = 0; i < this->texs.size(); i++) {
		SDL_DestroyTexture(this->texs[i]);
	}
	SDL_DestroyTexture(this->background);
}
