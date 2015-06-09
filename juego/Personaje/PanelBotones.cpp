#include "PanelBotones.hpp"
#include <iostream>
#include <SDL2/SDL_ttf.h>

#define DURACION_TEXTO 5*1000

using namespace std;

PanelBotones::PanelBotones(Parser* conf, SDL_Renderer *renderer, int numero_jugador){
	// cargamos todas las texturas relevantes para los botones
	// en un hash que lo llenamos la configuración
	unordered_map <string, int>* conf_joys = conf->joysticks->at(numero_jugador);

	// Acá van loops, pero no estoy en estado como para hacerlos

	imagenes_tomas[(*conf_joys)["pinia_baja"]] = IMG_LoadTexture(renderer, "resources/botones/c.png");
	imagenes_tomas[(*conf_joys)["cubrirse"]] = IMG_LoadTexture(renderer, "resources/botones/ci.png");
	imagenes_tomas[(*conf_joys)["patada_baja"]] = IMG_LoadTexture(renderer, "resources/botones/x.png");
	imagenes_tomas[(*conf_joys)["pinia_alta"]]	 = IMG_LoadTexture(renderer, "resources/botones/t.png");
	imagenes_tomas[(*conf_joys)["arrojar_arma"]] = IMG_LoadTexture(renderer, "resources/botones/x.png");
	imagenes_tomas[(*conf_joys)["patada_alta"]] = IMG_LoadTexture(renderer, "resources/botones/x.png");

	LTexture* imagen = new LTexture(renderer);
	imagen->loadFromFile("resources/botones/c.png", 0, 359, 180, false);
	imagenes_tomas_cambiadas[(*conf_joys)["pinia_baja"]] = imagen->mTexture;
	delete imagen;

	imagen = new LTexture(renderer);
	imagen->loadFromFile("resources/botones/ci.png", 0, 359, 180, false);
	imagenes_tomas_cambiadas[(*conf_joys)["cubrirse"]] = imagen->mTexture;
	delete imagen;
	imagen = new LTexture(renderer);
	imagen->loadFromFile("resources/botones/x.png", 0, 359, 180, false);
	imagenes_tomas_cambiadas[(*conf_joys)["patada_baja"]] = imagen->mTexture;
	delete imagen;
	imagen = new LTexture(renderer);
	imagen->loadFromFile("resources/botones/t.png", 0, 359, 180, false);
	imagenes_tomas_cambiadas[(*conf_joys)["pinia_alta"]] = imagen->mTexture;
	delete imagen;
	imagen = new LTexture(renderer);
	imagen->loadFromFile("resources/botones/x.png", 0, 359, 180, false);
	imagenes_tomas_cambiadas[(*conf_joys)["arrojar_arma"]] = imagen->mTexture;
	delete imagen;
	imagen = new LTexture(renderer);
	imagen->loadFromFile("resources/botones/x.png", 0, 359, 180, false);
	imagenes_tomas_cambiadas[(*conf_joys)["patada_alta"]] = imagen->mTexture;
	delete imagen;

	tiempo_max_boton = conf->tiempo_max_boton*1000;
	maximos_botones = conf->maximos_botones;
	errores_maximo = conf->errores_maximo;
}

PanelBotones::~PanelBotones(){
	for(auto boton: botones_actuales){
		delete boton;
	} 
	for(auto textura: imagenes_tomas){
		SDL_DestroyTexture(textura.second);
	}
	for(auto textura: imagenes_tomas_cambiadas){
		SDL_DestroyTexture(textura.second);
	} 
}

void PanelBotones::dibujar(ConversorDeCoordenadas* conv, SDL_Renderer *renderer){
	int off=0;

	while (botones_actuales.size() > maximos_botones){
		botones_actuales.erase(botones_actuales.begin());
	}
	int inicio = (conv->ancho_fisico/2) - (78*botones_actuales.size()/2);

	if (ejecutando_toma){
		if (SDL_GetTicks() - tiempo_toma < DURACION_TEXTO){
			// dibujamos el nombre de la toma
            TTF_Font* Sans = TTF_OpenFont("resources/miscelaneo/Mk3.ttf", 24);
			SDL_Color White = {255, 255, 255};
			SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, "Nombre de toma", White);
			SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

			SDL_Rect Message_rect;
			Message_rect.x = (conv->ancho_fisico/2) - surfaceMessage->w/2;
			Message_rect.y = 200;
			Message_rect.w = surfaceMessage->w;
			Message_rect.h = surfaceMessage->h;

			SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

			SDL_FreeSurface(surfaceMessage);
            SDL_DestroyTexture(Message);
        } else {
        	puts("ejecutando_toma en false");
        	ejecutando_toma = false;
        }
	}

	for (unsigned int i = 0; i < botones_actuales.size(); ++i){
	    /// std::cout << botones_actuales.at(i)->numero_boton << ' ';
		SDL_Rect destino;
		destino.x = inicio + off;
		destino.y = 100;
		destino.h = 78;
		destino.w = 78;
		if (botones_actuales.at(i)->otro_color){
			SDL_RenderCopy(renderer, imagenes_tomas_cambiadas[botones_actuales.at(i)->numero_boton], NULL, &destino);
		} else {
			SDL_RenderCopy(renderer, imagenes_tomas[botones_actuales.at(i)->numero_boton], NULL, &destino);
		}
		
		off += 78;

		if (!ejecutando_toma){
			if ((SDL_GetTicks() - botones_actuales.at(i)->inicio) > tiempo_max_boton){
				delete botones_actuales.at(i);
				botones_actuales.erase(botones_actuales.begin() + i--);
			}
		}
	}

	/// puts("");
}

void PanelBotones::AgregarBotones(int boton){
	if (!ejecutando_toma){
		EstructuraBoton* boton_temp = new EstructuraBoton();
		boton_temp->numero_boton = boton;
		boton_temp->inicio = SDL_GetTicks();
		botones_actuales.push_back(boton_temp);
	}
};

bool PanelBotones::checkToma(string toma){
	int tolerancia = this->errores_maximo;
	if (!ejecutando_toma){
		int i = 0, j = 0, errores = 0, aciertos = 0;

		if (!botones_actuales.size() || !toma.size()){
			/// std::cout << "No agarró toma1; " << ' ';
			return false;
		}
		
		while (true){
			if (j == botones_actuales.size()){
				/// std::cout << "Nunca encontré el primero; " << ' ';
				limpiar_botones();
				return false;
			}

			if (toma[0] - '0' == botones_actuales.at(j)->numero_boton){
				int h = j;
				i = 0;
				while (i < botones_actuales.size() && h < botones_actuales.size()){
					if (errores > tolerancia){
						/// std::cout << "No agarró toma2; " << ' ';
						// Tengo que des marcar todos los que tengo que cambiar
						limpiar_botones();
						//return false;
						break;
					}
					if (toma[i] - '0' == botones_actuales.at(h)->numero_boton){
						botones_actuales.at(h)->otro_color = true;
						i++;
						h++;
						aciertos++;
					} else {
						h++;
						errores++;
					}
					if (aciertos == toma.size()){
						/// std::cout << "Agarró toma; " << ' ';
						// tengo que marcar los que tengo que cambiar
						tiempo_toma = SDL_GetTicks();
						ejecutando_toma = true;
						return true;
					}
				}
			}
			// busco el primero en la siguiente posición del vector
			// y reinicio todos los indicadores
			j++;
			errores = 0;
			aciertos = 0;
			// break;
		}

		// Tengo que des marcar todos los que tengo que cambiar
		limpiar_botones();
		/// std::cout << "no Agarró toma 3; " << ' ';
		return false;
	}
	return false;
}

void PanelBotones::limpiar_botones(){
	for (unsigned int i = 0; i < botones_actuales.size(); i++){
		botones_actuales.at(i)->otro_color = false;
	}
}
