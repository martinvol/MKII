#ifndef PANELBOTONES_H_INCLUDED
#define PANELBOTONES_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <unordered_map>
#include "../Coordenadas/ConversorDeCoordenadas.hpp"
#include "../Parser/Parser.hpp"
#include <string>

using namespace std;

class EstructuraBoton{
	public:
		int numero_boton;
		Uint32 inicio;

		EstructuraBoton(){};
		~EstructuraBoton(){};
	private:
};

class PanelBotones {
	public:
		PanelBotones(Parser* conf, SDL_Renderer *renderer, int numero_jugador); // un conversor de coordenadas, 
		~PanelBotones();
		
		void AgregarBotones(int boton);
		void dibujar(ConversorDeCoordenadas* conv, SDL_Renderer *renderer); // recibe un vector con la lista de cosas a mostrar
		void checkToma(string toma);

	private:
		std::vector<EstructuraBoton*> botones_actuales;
		unordered_map <int, SDL_Texture*> imagenes_tomas;
		float tiempo_max_boton, maximos_botones;
};

#endif
