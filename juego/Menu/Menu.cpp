#include "Menu.hpp"

/************************************************************
*	 						BOTON							*
************************************************************/
Boton::Boton(modo mode, int ancho_fisico, int alto_fisico, int x, int y, SDL_Renderer* renderer){
	this->renderer = renderer;
	if (mode == Pelea){
		boton = IMG_LoadTexture(renderer, "resources/menu/Pelea.png");
	} else
	if (mode == Practica){
		boton = IMG_LoadTexture(renderer, "resources/menu/Practica.png");
	} else
	//if (mode == CPU)
	{
		boton = IMG_LoadTexture(renderer, "resources/menu/CPU.png");
	}
	
	this->destino.x = x;
	this->destino.y = y;
	this->destino.w = ancho_fisico;
	this->destino.h = alto_fisico;
}

Boton::~Boton(){
	SDL_DestroyTexture(boton);
}

void Boton::Dibujarse(SDL_Texture* seleccion){
	SDL_RenderCopy(renderer, boton, NULL, &destino);
	if (seleccion != NULL){
		SDL_RenderCopy(renderer, seleccion, NULL, &destino);
	}
}

SDL_Rect Boton::obtenerPosicionEnVentana(){
	return destino;
}



/************************************************************
*	 						MENU							*
************************************************************/

Menu::Menu(SDL_Renderer* renderer, Ventana* ventana){
	this->renderer = renderer;
	fondo = IMG_LoadTexture(renderer, "resources/menu/fondo3.png");
	opcion = IMG_LoadTexture(renderer, "resources/menu/opcion_transparent.png");
	seleccion = IMG_LoadTexture(renderer, "resources/menu/seleccion_transparent.png");
	
	ancho_fisico = ventana->ancho_fisico;
	alto_fisico = ventana->alto_fisico;
	idVentana = SDL_GetWindowID(ventana->window);
	
	modo_actual = CPU;
	seleccionado = false;
	
	int ancho_fisico_botones = ancho_fisico / 2.0;
	int alto_fisico_botones = alto_fisico / 6.0;
	int offset_entre_botones = alto_fisico_botones / 3.0;
	int x = (ancho_fisico / 2.0) - (ancho_fisico_botones / 2.0);
	int y = alto_fisico - 2*alto_fisico_botones;
	botones.push_back(new Boton(Pelea, ancho_fisico_botones, alto_fisico_botones, x, y, renderer));
	y = y - offset_entre_botones - alto_fisico_botones;
	botones.push_back(new Boton(Practica, ancho_fisico_botones, alto_fisico_botones, x, y, renderer));
	y = y - offset_entre_botones - alto_fisico_botones;
	botones.push_back(new Boton(CPU, ancho_fisico_botones, alto_fisico_botones, x, y, renderer));
}

Menu::~Menu(){
	SDL_DestroyTexture(fondo);
	SDL_DestroyTexture(opcion);
	SDL_DestroyTexture(seleccion);
	for (unsigned int i = 0; i < botones.size(); i++){
		delete botones[i];
	}
	botones.clear();
}

void Menu::Dibujarse(){
	SDL_RenderCopy(renderer, fondo, NULL, NULL);
	for(int i=0;i<botones.size();i++){
		if (i == (int) modo_actual){
			if (seleccionado){
				botones[i]->Dibujarse(this->seleccion);
			} else {
				botones[i]->Dibujarse(this->opcion);
			}
		} else {
			botones[i]->Dibujarse(NULL);
		}
	}
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
	seleccionado = true;
	return modo_actual;
}

vector<SDL_Rect> Menu::obtenerPosicionesBotones(){
	vector<SDL_Rect> vector;
	for(int i=0;i<botones.size();i++){
		vector.push_back(botones[i]->obtenerPosicionEnVentana());
	}
	return vector;
}

SDL_Renderer* Menu::obtenerRenderer(){
	return renderer;
}

Uint32 Menu::obtenerIDventana(){
	return idVentana;
}

void Menu::apuntarAOpcion(modo mode){
	modo_actual = mode;
}
