#ifndef GRILLA_H_INCLUDED
#define GRILLA_H_INCLUDED

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "../Director/Director.hpp"

using namespace std;

class Grilla {
	private:
		SDL_Renderer* ren;
		vector<string> paths;
	public:
		Grilla(SDL_Renderer* renderer);
		void Dibujarse();
		string obtenerPathJugadorNumero(num_jugador jugador);		
}

#endif
