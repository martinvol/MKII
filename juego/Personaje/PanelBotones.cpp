#ifndef PANELBOTONES_H_INCLUDED
#define PANELBOTONES_H_INCLUDED

#include "PanelBotones.hpp"
#include <iostream>

using namespace std;



PanelBotones::PanelBotones(unordered_map <string, int>* conf_joys, SDL_Renderer *renderer){
	// cargamos todas las texturas relevantes para los botones
	// en un hash que lo llenamos la configuración
	imagenes_tomas[(*conf_joys)["pinia_baja"]] = IMG_LoadTexture(renderer, "resources/botones/c.png");
	imagenes_tomas[(*conf_joys)["cubrirse"]] = IMG_LoadTexture(renderer, "resources/botones/ci.png");
	imagenes_tomas[(*conf_joys)["patada_baja"]] = IMG_LoadTexture(renderer, "resources/botones/x.png");
	imagenes_tomas[(*conf_joys)["pinia_alta"]]	 = IMG_LoadTexture(renderer, "resources/botones/t.png");
	imagenes_tomas[(*conf_joys)["arrojar_arma"]] = IMG_LoadTexture(renderer, "resources/botones/x.png");
	imagenes_tomas[(*conf_joys)["patada_alta"]] = IMG_LoadTexture(renderer, "resources/botones/x.png");
}

PanelBotones::~PanelBotones(){
	for(auto texture: imagenes_tomas){
		SDL_DestroyTexture(texture.second);
	} 
}

void PanelBotones::dibujar(ConversorDeCoordenadas* conv, SDL_Renderer *renderer){
	int off=0;
	int cantidad_max=5;

	while (botones_actuales.size() > cantidad_max){
		botones_actuales.erase(botones_actuales.begin());
	}

	for (unsigned int i = 0; i < botones_actuales.size(); ++i){
	    std::cout << botones_actuales.at(i)->numero_boton << ' ';
		SDL_Rect destino;
		destino.x = 100 + off;
		std::cout << destino.x << ' ';
		destino.y = 100;
		destino.h = 78;
		destino.w = 78;
		SDL_RenderCopy(renderer, imagenes_tomas[botones_actuales.at(i)->numero_boton], NULL, &destino);
		off += 78;
		
		if ((SDL_GetTicks() - botones_actuales.at(i)->inicio) > 5000){
			botones_actuales.erase(botones_actuales.begin() + i--);
		}
	}
	puts("");
}

void PanelBotones::AgregarBotones(int boton){
	EstructuraBoton* boton_temp = new EstructuraBoton();
	boton_temp->numero_boton = boton;
	boton_temp->inicio = SDL_GetTicks();
	botones_actuales.push_back(boton_temp);
};

#endif
