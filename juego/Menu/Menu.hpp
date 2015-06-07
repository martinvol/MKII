#ifndef MENU_H
#define MENU_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../Coordenadas/ConversorDeCoordenadas.hpp"
#include "../Escenario/Ventana.hpp"
using namespace std;


typedef enum modo {Pelea = 0, Practica, CPU, MENU} modo;

class BotonMenu{
	private:
		SDL_Texture* boton;
		SDL_Rect destino;
		SDL_Renderer* renderer;
		
	public:
	
		BotonMenu(modo mode, int ancho_fisico, int alto_fisico, int x, int y, SDL_Renderer* renderer);
		~BotonMenu();
		
		void Dibujarse(SDL_Texture* seleccion);
		SDL_Rect obtenerPosicionEnVentana();

};

class Menu {
	private:
		
		modo modo_actual;
		vector<BotonMenu*> botones;
		SDL_Renderer* renderer;
		SDL_Texture* fondo;
		SDL_Texture* opcion, *seleccion;
		int ancho_fisico, alto_fisico;
		bool seleccionado;
		Uint32 idVentana;
		
	public:	
		Menu(SDL_Renderer* renderer, Ventana* ventana);
		~Menu();
		
		void Dibujarse();
		void subirOpcion();
		void bajarOpcion();
		modo seleccionarOpcion();	
		
		/* Para el Controlador */
		vector<SDL_Rect> obtenerPosicionesBotones();
		SDL_Renderer* obtenerRenderer();
		Uint32 obtenerIDventana();
		void apuntarAOpcion(modo mode);
};

#endif //MENU_H
