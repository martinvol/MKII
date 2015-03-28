#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;

const int MAX_NUM_CUADROS 9;

class Personaje {
		int posicion_x;
		int posicion_y;
		int url;		
		int estado;
		int modo;
		int currentTime;
		SDL_Surface* imagenes[MAX_NUM_CUADROS];
	public:
		void definir_imagen(int accion);
		void mover_personaje();
		int cuadro(); //consultor
		
};

