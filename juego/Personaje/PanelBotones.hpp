#ifndef PANELBOTONES_H_INCLUDED
#define PANELBOTONES_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <unordered_map>
#include "../Coordenadas/ConversorDeCoordenadas.hpp"
#include "../LTexture.hpp"
#include "../Parser/Parser.hpp"
#include "TomaData.hpp"
#include <string>
#include <SDL2/SDL_ttf.h>

using namespace std;

class EstructuraBoton{
	public:
		char numero_boton;
		Uint32 inicio;
		bool otro_color = false;

		EstructuraBoton(){};
		~EstructuraBoton(){};
	private:
};

class PanelBotones {
	public:
		PanelBotones(Parser* conf, SDL_Renderer *renderer, int numero_jugador, bool debo_dibujar); // un conversor de coordenadas, 
		~PanelBotones();
		
		void AgregarBotones(char boton);
		void dibujar(ConversorDeCoordenadas* conv, SDL_Renderer *renderer); // recibe un vector con la lista de cosas a mostrar
		bool checkToma(string, string);

	private:
		std::vector<EstructuraBoton*> botones_actuales;
		unordered_map <int, SDL_Texture*> imagenes_tomas;
		unordered_map <int, SDL_Texture*> imagenes_tomas_cambiadas;
		string nombre_toma;
		float tiempo_max_boton, tiempo_toma = 0;
		int maximos_botones, errores_maximo;
		bool ejecutando_toma = false;
		void limpiar_botones();
		bool debo_dibujar;
		TTF_Font* Sans;
};

#endif
