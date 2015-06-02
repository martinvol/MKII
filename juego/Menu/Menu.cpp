#include "Menu.hpp"


Menu::Menu(SDL_Renderer* renderer, int ancho_fis_ventana, int alto_fis_ventana){
	this->renderer = renderer;
	fondo = IMG_LoadTexture(renderer, "resources/menu/fondo.png");
	ancho_fisico = ancho_fis_ventana;
	alto_fisico = alto_fis_ventana;
	
	modo_actual = CPU;
	
	modos.push_back("2 jugadores");
	modos.push_back("Práctica");
	modos.push_back("Jugador VS CPU");
}

Menu::~Menu(){
	SDL_DestroyTexture(fondo);
}

void Menu::Dibujarse(){
	//~ SDL_Rect source;
	//~ source.x = 0;
	//~ source.y = 0;
	//~ source.w = ancho_fisico;
	//~ source.h = alto_fisico;
	
	//~ SDL_RenderCopy(renderer, fondo, &source, NULL);
	SDL_RenderCopy(renderer, fondo, NULL, NULL);
}

void Menu::subirOpcion(){
	modo_actual = (modo) ((modo_actual+1) % 3);
}

void Menu::bajarOpcion(){
	// Porque -1%3 = -1
	if (modo_actual == Pelea){
		modo_actual = CPU;
	} else {
		modo_actual = (modo) ((modo_actual-1) % 3);
	}
}

modo Menu::seleccionarOpcion(){
	return modo_actual;
}
