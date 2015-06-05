#ifndef PANELBOTONES_H_INCLUDED
#define PANELBOTONES_H_INCLUDED

#include "PanelBotones.hpp"
#include <iostream>

using namespace std;

PanelBotones::PanelBotones(){

	// cargamos todas las texturas relevantes para los botones
	// en un hash que lo llenamos con el conversor

}

PanelBotones::~PanelBotones(){

}

void PanelBotones::dibujar(ConversorDeCoordenadas* conv, SDL_Renderer *renderer){
	int off=0;
	for (auto i = botones_actuales.begin(); i != botones_actuales.end(); ++i){
	    std::cout << *i << ' ';
		SDL_Texture *texturaAux = IMG_LoadTexture(renderer, "resources/botones/x.png");
		SDL_Rect destino;
		destino.x = 100 + off;
		std::cout << destino.x << ' ';
		destino.y = 100;
		destino.h = 78;
		destino.w = 78;
		SDL_RenderCopy(renderer, texturaAux, NULL, &destino);
		SDL_DestroyTexture(texturaAux);
		off += 78;
	 }
	puts("");
}

void PanelBotones::AgregarBotones(int boton){
	botones_actuales.push_back(boton);
};

#endif
