#ifndef PERSONAJE_H_INCLUDED
#define PERSONAJE_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "Accion.hpp"

using namespace std;

class Personaje {
	
		int posicion_x;
		int posicion_y;
		string nombrePersonaje; 	//Coincide con el nombre de la carpeta.		
		Accion* accionActual;
		int lastTime;
		SDL_Texture* imagenActual;
		SDL_Renderer* renderer;
		
	public:
		
		Personaje(int posicion_x, int posicion_y, string nombre,SDL_Renderer* ren);
		SDL_Texture* definir_imagen(int accion);
		void cambiar_posicion(int cant_pasos_x,int cant_pasos_y);
		void mirar_al_otro_lado();
		void cambiarAccionA(int nroACcion,string ruta, bool permiteInterrupcion);
		void destruirPersonaje();
		void Dibujarse(int x, int y);
		void Dibujarse(int x, int y, int alto, int ancho);
};

#endif
