#ifndef PERSONAJE_H_INCLUDED
#define PERSONAJE_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;

#define MAX_NUM_CUADROS 9

class Personaje {
	
		int posicion_x;
		int posicion_y;
		int url;		
		int estado;
		int modo;
		int lastTime;
		SDL_Texture* imagenes[MAX_NUM_CUADROS];
	public:
		void personaje(int posicion_x, int posicion_y);
		void inicializar_movimiento_personaje(int estado, SDL_Renderer* ren);
		void reiniciar();
		SDL_Texture* definir_imagen(int accion, SDL_Renderer* ren);
		void cambiar_posicion(int cant_pasos_x,int cant_pasos_y);
		void mirar_al_otro_lado();
		//int cuadro(); //consultor
		
};

#endif
