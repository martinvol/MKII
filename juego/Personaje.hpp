#ifndef PERSONAJE_H_INCLUDED
#define PERSONAJE_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "Estado.hpp"
#include "Accion.hpp"
#include "CoordenadaLogica.hpp"
#include "Parser.hpp"

using namespace std;

class Personaje {
	
	private:
		/* Coordenada Logica de la esquina izquierda inferior. */	
		CoordenadaLogica* coordenada;
		/* En unidades logicas. */
		float alto;
		float ancho;
		
		Parser* parser;
		string nombrePersonaje; 	//Coincide con el nombre de la carpeta.		
		Accion* accionActual;
		Estado* estado;
		bool ladoDerecha;
		int lastTime;
		SDL_Texture* imagenActual;
		SDL_Renderer* renderer;
		int nroAccionActual;
		
		//void Dibujarse(int x, int y, int alto, int ancho);
		
	public:
		
		Personaje(CoordenadaLogica* coord, string nombre,SDL_Renderer* ren, string ruta);
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

		//MILE: A agregar para DANI:
		
		/* Sólo espejan o no espejan. */
		void mirarParaDerecha();
		void mirarParaIzquierda();
		
		/* Sólo te dicen a que acción te están pidiendo que cambies.
		 * Sólo lo debés hacer si es válido interrumpir tu acción
		 * para la que te dicen; sino, no cambiás nada. */
		void parar();
		void caminarDerecha();
		void caminarIzquierda();
		void saltar();
		void saltarDerecha();
		void saltarIzquierda();
		
		/* Devuelven coordenadas que deben ser liberadas para no perder
		 * memoria. */
		CoordenadaLogica* obtenerCoordenadaIzqSup();
		CoordenadaLogica* obtenerCoordenadaIzqInf();
		CoordenadaLogica* obtenerCoordenadaDerSup();
		CoordenadaLogica* obtenerCoordenadaDerInf();
		
		/* Estas corresponden a preguntar dónde deberías estar o a
		 * dónde te deberías mover la próxima vez de acuerdo a la
		 * acción que está en transcurso. */
		 /* Devuelven coordenadas que deben ser liberadas para no perder
		 * memoria. */
		CoordenadaLogica* obtenerSiguienteCoordenadaIzqSup();
		CoordenadaLogica* obtenerSiguienteCoordenadaIzqInf();
		CoordenadaLogica* obtenerSiguienteCoordenadaDerSup();
		CoordenadaLogica* obtenerSiguienteCoordenadaDerInf();
		
		/* Coordenada a la que debe moverse el pesonaje. */
		void moverseAIzqSup(CoordenadaLogica* coord);
		void moverseADerSup(CoordenadaLogica* coord);
		
};



#endif
