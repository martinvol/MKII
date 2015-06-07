#ifndef GRILLA_H_INCLUDED
#define GRILLA_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>

using namespace std;

class Grilla {
	private:
		SDL_Renderer* ren;
		vector<string> paths;
		vector<SDL_Texture*> texs;
		vector<SDL_Texture*> seleccion;
		void cargarTexturas();
		int anchoImagen, altoImagen;
		int xSeleccion1, ySeleccion1;
		int xSeleccion2, ySeleccion2;
		SDL_Texture* background;
	public:
		//Grilla(SDL_Renderer* renderer, Ventana* ventana);
		Grilla(SDL_Renderer* renderer);
		void Dibujarse();
		string obtenerPathJugadorNumero(int jugador);
		~Grilla();
};

#endif
