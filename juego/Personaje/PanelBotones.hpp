#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "../Coordenadas/ConversorDeCoordenadas.hpp"

using namespace std;

class PanelBotones {

public:
	PanelBotones(); // un conversor de coordenadas, 
	~PanelBotones();
	
	void AgregarBotones(int boton);
	void dibujar(ConversorDeCoordenadas* conv, SDL_Renderer *renderer); // recibe un vector con la lista de cosas a mostrar

private:
	std::vector<int> botones_actuales;
};