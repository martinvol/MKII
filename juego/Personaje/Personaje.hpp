#ifndef PERSONAJE_H_INCLUDED
#define PERSONAJE_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "Estado.hpp"
#include "../Acciones/Accion.hpp"
#include "../Coordenadas/CoordenadaLogica.hpp"
#include "../Coordenadas/ConversorDeCoordenadas.hpp"
#include "../Parser/Parser.hpp"

using namespace std;

class Personaje {
	
	private:
		/* Sus anchos y altos lógicos. */
		float ancho;
		float alto;
		
		/* Para saltos */
		float y_inicial;
		
		/* Coordenada inferior izquierda. */
		CoordenadaLogica* coordenada;
		CoordenadaLogica* siguiente;
		
		string nombrePersonaje; 	//Coincide con el nombre de la carpeta.		
		
		Accion* accionActual;
		Estado* estado;
		
		int nroAccionActual;
		bool mirarDerecha;
		
		SDL_Texture* imagenActual;
		SDL_Renderer* renderer;
		
		accion_posible nroAccionActual;
		
	public:
		//Harcodeo para donde mira.
		Personaje(CoordenadaLogica* coord, string nombre,SDL_Renderer* ren, float ancho, float alto, Estado* estado, bool derecha);
		//~ Después debería ser
		//~ Personaje::Personaje(CoordenadaLogica* coord, string nombre,SDL_Renderer* ren, float alto, float ancho, Estado* estado){
		
		~Personaje();
		
		/* VIEJO
		void definir_imagen(accion_posible accion);
		//~ void cambiar_posicion(int cant_pasos_x,int cant_pasos_y);
		//~ void mirar_al_otro_lado();
		void cambiarAccionA(accion_posible nroAccion);
		void Dibujarse(float alto, float ancho);
		//void Dibujarse(int x, int y, int alto, int ancho);
		void Dibujarse(int x, int y, float alto, float ancho); // Esto esta sobrecargado *Manu*
		//~ int getSpriteActual();
		//Para Maxi
		//SDL_Texture* DibujarSpriteNumero(int numeroDeSprite);
		*/


		// Dibujarse que debería usarse finalmente
		void Dibujarse(ConversorDeCoordenadas* conv);

		/* Sólo espejan o no espejan. */
		void mirarParaDerecha();
		void mirarParaIzquierda();
		
		/* Sólo te dicen a que acción te están pidiendo que cambies.
		 * Sólo lo debés hacer si es válido interrumpir tu acción
		 * para la que te dicen; sino, no cambiás nada. */
		void activarAccion(accion_posible accion);
		
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
		/* Se queda con la coordenada, no la deben liberar por afuera. */
		void moverseAIzqSup(CoordenadaLogica* coord);
		void moverseADerSup(CoordenadaLogica* coord);
		
};



#endif
