#ifndef MENU_H
#define MENU_H

#include <vector>

#include "../Personaje/Personaje.hpp"

typedef enum modo {Pelea = 0, Practica, CPU, MENU} modo;

class Menu {
	private:
		vector<Personaje*> personajes;
		vector<string> modos;
		modo modo_actual;
		SDL_Renderer* renderer;
		SDL_Texture* fondo;
		int ancho_fisico, alto_fisico;
		
	public:
		Menu(SDL_Renderer* renderer, int ancho_fis_ventana, int alto_fis_ventana);
		~Menu();
		
		void Dibujarse();
		void subirOpcion();
		void bajarOpcion();
		modo seleccionarOpcion();	
};

#endif //MENU_H
