#ifndef LATIGO_H_INCLUDED
#define LATIGO_H_INCLUDED

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

		Latigo(SDL_Renderer* ren, int alto, int ancho);

		// desde donde
		// ESTO se encarga de liberar la memoria de la coordenada que le pasan
		// ANCHO y ALTO fisicos
		
		void tirar(CoordenadaLogica *coord, float velocidad_arma, float alto_personaje, float ancho_personaje);
		void dibujar(ConversorDeCoordenadas* conv);
		/*void tirar(float velocidad_arma);
		

		bool termino(ConversorDeCoordenadas *conv);*/

		~Latigo();

		Rectangulo * rectangulo;
		bool pego=false;
		CoordenadaLogica *coord=NULL;
		bool volver = false;
		bool espejado;
	private:
		SDL_Texture* imagen;
		SDL_Renderer* ren;
		int alto, ancho, alto_original, ancho_original;
		int nuevo_ancho;
		float vel_horizontal;
};


#endif
