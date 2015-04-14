#ifndef CAPA_H_INCLUDED
#define CAPA_H_INCLUDED

#include "ConversorDeCoordenadas.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "ConversorDeCoordenadas.hpp"

using namespace std;

class Capa{
// private:
	public:
		SDL_Renderer *ren;
		string ubicacion;
		float anchoLogico;
		float x_logico;
		
		float b, m;

		Capa (string ubicacionParam, float anchoLogicoParam, float x_logicoParam, SDL_Renderer *rendererParam, float ancho_escenario, float ancho_ventana_logico);
		~Capa ();
		void Dibujarse(int x, int y);
		void Dibujarse(int x, int y, int alto, int ancho);
		void Dibujarse2(int x, int y, ConversorDeCoordenadas* conversor);
		void DibujarseAnchoReal(int x, int y, ConversorDeCoordenadas* conversor);
		void DibujarseAnchoReal2(int x, int y, ConversorDeCoordenadas* conversor);
		SDL_Texture* CargarTextura();

		SDL_Texture* textura;
		ConversorDeCoordenadas* conversor;
};
#endif // CAPA_H_INCLUDED
