#ifndef ARROBAJE_H_INCLUDED
#define ARROBAJE_H_INCLUDED

#define TIRAR_ARRIBA true
#define TIRAR_ABAJO false

#include <SDL2/SDL.h>
#include <string>
#include <SDL2/SDL_image.h>

using namespace std;


class Arrojable{
	public:

		Arrojable(string ruta, bool derecha, SDL_Renderer* ren);

		// desde donde
		void set_coordenadas(float x, float y, float alto, float ancho);
		
		void tirar();
		
		void tirar_diagonal(bool arriba);

		void dibujar();

		bool salioDeLaPantalla(float tamanio_escenario);

		~Arrojable();

	private:
		SDL_Renderer* ren;
		SDL_Texture* imagen;
		float alto, ancho;
		float x,y;
		float vel_horizontal, vel_vertical;
};


#endif
