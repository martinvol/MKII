#ifndef FATALITY_H_INCLUDED
#define FATALITY_H_INCLUDED

#include "../Personaje.hpp"
#include "Perform.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


class Fatality {
	
	public:
		Personaje* victima;
		Personaje* victimario;
		Perform* acido;
		Perform* desenmascararse;
		SDL_Texture* texAcidoActual;
		bool execute();
		Fatality(Personaje* victimario, Personaje* victima, string path);
		~Fatality();
		int anchoVictima, altoVictima;
		int x, y;
		int anchoPrimerAcido, altoPrimerAcido;
};

#endif
