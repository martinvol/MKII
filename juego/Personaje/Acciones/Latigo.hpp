#ifndef ARROBAJE_H_INCLUDED
#define ARROBAJE_H_INCLUDED

#define TIRAR_ARRIBA true
#define TIRAR_ABAJO false

#include <SDL2/SDL.h>
#include <string>
#include <SDL2/SDL_image.h>
#include "../../Coordenadas/CoordenadaLogica.hpp"
#include "../../Coordenadas/ConversorDeCoordenadas.hpp"
#include "Rectangulo.hpp"

using namespace std;


class Latigo{
	public:

		Latigo(bool derecha, SDL_Renderer* ren, int alto, int ancho);

		// desde donde
		// ESTO se encarga de liberar la memoria de la coordenada que le pasan
		// ANCHO y ALTO fisicos
		void setCoordenadas(CoordenadaLogica *coord, int alto, int ancho);
		
		void tirar(float velocidad_arma);
		
		void dibujar(ConversorDeCoordenadas* conv);

		bool termino(ConversorDeCoordenadas *conv);

		~Latigo();

		Rectangulo * rectangulo;
		bool pego=false;
	private:
		SDL_Texture* imagen;
		SDL_Renderer* ren;
		int alto, ancho, alto_original;
		CoordenadaLogica *coord;
		float vel_horizontal, vel_vertical;
		bool espejado;
};


#endif
