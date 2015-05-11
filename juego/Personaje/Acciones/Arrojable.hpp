#ifndef ARROBAJE_H_INCLUDED
#define ARROBAJE_H_INCLUDED

#define TIRAR_ARRIBA true
#define TIRAR_ABAJO false

#include <SDL2/SDL.h>
#include <string>
#include <SDL2/SDL_image.h>
#include "../../Coordenadas/CoordenadaLogica.hpp"
#include "../../Coordenadas/ConversorDeCoordenadas.hpp"

using namespace std;


class Arrojable{
	public:

		Arrojable(string ruta, bool derecha, SDL_Renderer* ren);

		// desde donde
		void setCoordenadas(CoordenadaLogica *coord, float alto, float ancho);
		
		void tirar();
		
		void tirarDiagonal(bool arriba);

		void dibujar(ConversorDeCoordenadas* conv);

		bool salioDeLaPantalla(float tamanio_escenario);

		~Arrojable();

	private:
		SDL_Texture* imagen;
		SDL_Renderer* ren;
		float alto, ancho;
		CoordenadaLogica *coord;
		float vel_horizontal, vel_vertical;
};


#endif
