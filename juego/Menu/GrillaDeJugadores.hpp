#ifndef GRILLA_H_INCLUDED
#define GRILLA_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include "../Personaje/Acciones/Accion.hpp"
#include "../Personaje/Acciones/Quieto.hpp"
#include "../Personaje/Acciones/Ganar.hpp"

using namespace std;

class Grilla {
	private:
		SDL_Renderer* ren;
		vector<string> paths;
		vector<SDL_Texture*> texs;
		vector<SDL_Texture*> seleccion;
		vector<SDL_Texture*> numero;
		void cargarTexturas();
		int anchoImagen, altoImagen;
		vector<int> xSeleccion;
		vector<int> ySeleccion;
		//int xSeleccion1, ySeleccion1;
		//int xSeleccion2, ySeleccion2;
		SDL_Texture* background;
		int obtenerUbicacion(int x, int y);
		string obtenerPath(int pj);
		vector<Accion*> accionesQuieto;
		vector<Accion*> accionesGanar;
		
		SDL_Texture* header;
		
		int anchoVentana, altoVentana;
		int x_init, y_init;
	public:
		//Grilla(SDL_Renderer* renderer, Ventana* ventana);
		Grilla(SDL_Renderer* renderer, int anchoVentana, int altoVentana);
		void Dibujarse();
		~Grilla();
		vector<bool> eligio;
		/* Para el controller */
		// int jugador: 0 o 1
		void subirOpcion(int jugador);
		void bajarOpcion(int jugador);
		void moverDerechaOpcion(int jugador);
		void moverIzquierdaOpcion(int jugador);
		string seleccionarOpcion(int jugador);
};

#endif
