#ifndef PERSONAJE_H_INCLUDED
#define PERSONAJE_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "Estado.hpp"
#include "Accion.hpp"
#include "CoordenadaLogica.h"
#include "parser.h"

using namespace std;

class Personaje {
		Conf* parser;
		int posicion_x;
		int posicion_y;
		string nombrePersonaje; 	//Coincide con el nombre de la carpeta.		
		Accion* accionActual;
		Estado* estado;
		int lastTime;
		SDL_Texture* imagenActual;
		SDL_Renderer* renderer;
		//void Dibujarse(int x, int y, int alto, int ancho);
		
	public:
		
		Personaje(int posicion_x, int posicion_y, string nombre,SDL_Renderer* ren, Conf* parser);
		~Personaje();
		void definir_imagen(float tmp, int accion);
		void cambiar_posicion(int cant_pasos_x,int cant_pasos_y);
		void mirar_al_otro_lado();
		void cambiarAccionA(int nroACcion);
		void Dibujarse(int x, int y);
		//void Dibujarse(int x, int y, int alto, int ancho);
		void Dibujarse(int x, int y, float alto, float ancho); // Esto esta sobrecargado *Manu*
		int getSpriteActual();
		//Para Maxi
		//SDL_Texture* DibujarSpriteNumero(int numeroDeSprite);

		//MILE: A agregar:
		
		CoordenadaLogica* obtenerCoordenada();
		/* Sólo espejan o no espejan. */
		void mirarParaDerecha();
		void mirarParaIzquierda();
		/* Devuelve la coordenada en la que está.
		 * Comienza (o sigue) la imagen de parar. */
		CoordenadaLogica* parar();
		/* Devuelve la coordenada a la que iría.
		 * Comienza (o sigue) la imagen de saltar. */
		CoordenadaLogica* saltar();
		/* Devuelve la coordenada a la que iría.
		 * Comienza (o sigue) la imagen de saltar para la derecha. */
		CoordenadaLogica* saltarDerecha();
		/* Devuelve la coordenada a la que iría.
		 * Comienza (o sigue) la imagen de saltar para la izquierda. */
		CoordenadaLogica* saltarIzquierda();
		/* Devuelve la coordenada a la que iría.
		 * Comienza (o sigue) la imagen de caminar para la derecha. */
		CoordenadaLogica* caminarDerecha();
		/* Devuelve la coordenada a la que iría.
		 * Comienza (o sigue) la imagen de caminar para la izquierda. */
		CoordenadaLogica* caminarIzquierda();
		
};



#endif
