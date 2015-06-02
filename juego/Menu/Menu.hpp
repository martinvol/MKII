#ifndef MENU_H
#define MENU_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../Coordenadas/ConversorDeCoordenadas.hpp"
using namespace std;


typedef enum modo {Pelea = 0, Practica, CPU, MENU} modo;

class Boton{
	private:
		SDL_Texture* boton;
		SDL_Rect destino;
		SDL_Renderer* renderer;
		
	public:
		Boton(modo mode, int ancho_fisico, int alto_fisico, int x, int y, SDL_Renderer* renderer);
		~Boton();
		
		void Dibujarse(SDL_Texture* seleccion);
};

class Menu {
	private:
		vector<Boton*> botones;
		modo modo_actual;
		SDL_Renderer* renderer;
		SDL_Texture* fondo;
		SDL_Texture* opcion, *seleccion;
		int ancho_fisico, alto_fisico;
		bool seleccionado;
		
	public:
		Menu(SDL_Renderer* renderer, int ancho_fis_ventana, int alto_fis_ventana);
		~Menu();
		
		void Dibujarse();
		void subirOpcion();
		void bajarOpcion();
		modo seleccionarOpcion();	
};

#endif //MENU_H
