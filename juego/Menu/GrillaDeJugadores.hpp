#ifndef GRILLA_H_INCLUDED
#define GRILLA_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include <cstdlib>
#include "../Personaje/Acciones/Accion.hpp"
#include "../Personaje/Acciones/Quieto.hpp"
#include "../Personaje/Acciones/Ganar.hpp"
#include "TextBox.hpp"

using namespace std;

class Grilla {
	public:
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
		
		TextBox* textbox1, *textbox2;
		
		SDL_Texture* header;
		
		bool entraEnGrilla(int x, int y);
		
		int anchoVentana, altoVentana;
		int x_init, y_init, x_header, y_header;
		Uint32 lastTick1;
		Uint32 lastTick2;
	
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
		
		Uint32 idVentana;
		
		void open(Uint32 idVentana);
		
		/*Eleccion para AI*/
		string randomChoicePlayer2();
};

#endif
