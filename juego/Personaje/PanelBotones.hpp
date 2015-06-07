#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <unordered_map>
#include "../Coordenadas/ConversorDeCoordenadas.hpp"
#include "../Parser/Parser.hpp"

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
		PanelBotones(unordered_map <string, int>* conf_joys, SDL_Renderer *renderer); // un conversor de coordenadas, 
		~PanelBotones();
		
		void AgregarBotones(int boton);
		void dibujar(ConversorDeCoordenadas* conv, SDL_Renderer *renderer); // recibe un vector con la lista de cosas a mostrar

	private:
		std::vector<EstructuraBoton*> botones_actuales;
		unordered_map <int, SDL_Texture*> imagenes_tomas;
};