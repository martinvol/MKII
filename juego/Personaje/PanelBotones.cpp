#ifndef PANELBOTONES_H_INCLUDED
#define PANELBOTONES_H_INCLUDED

#include "PanelBotones.hpp"
#include <iostream>

using namespace std;

class EstructuraBoton{
	public:
		int numero_boton;
		Uint32 inicio;

		/*PanelBotones();
		~PanelBotones();*/
	private:
};

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
	for (auto i = botones_actuales.begin(); i != botones_actuales.end(); ++i){
	    std::cout << *i << ' ';
		// SDL_Texture *texturaAux = IMG_LoadTexture(renderer, "resources/botones/x.png");
		SDL_Rect destino;
		destino.x = 100 + off;
		std::cout << destino.x << ' ';
		destino.y = 100;
		destino.h = 78;
		destino.w = 78;
		SDL_RenderCopy(renderer, imagenes_tomas[*i], NULL, &destino);
		off += 78;
	 }
	puts("");
}

void PanelBotones::AgregarBotones(int boton){
	/*EstructuraBoton* boton_temp = new PanelBotones();
	boton->numero_boton = boton;
	boton->inicio = SDL_GetTicks();*/

	botones_actuales.push_back(boton);
};

#endif
