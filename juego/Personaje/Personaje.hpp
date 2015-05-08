#ifndef PERSONAJE_H_INCLUDED
#define PERSONAJE_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "Estado.hpp"
#include "../Personaje/Acciones/Accion.hpp"
#include "../Coordenadas/CoordenadaLogica.hpp"
#include "../Coordenadas/ConversorDeCoordenadas.hpp"
#include "../Parser/Parser.hpp"
#include "../Escenario/BarraDeVida.hpp"


using namespace std;

class Personaje {
		//Conf* parser;
		
	public:
	
	//~ private:
		/* Sus anchos y altos lógicos. */
		float ancho;
		float alto;
		
		/* Para saltos */
		float y_inicial;
		
		/* Coordenada inferior izquierda. */
		CoordenadaLogica* coordenada;
		CoordenadaLogica* siguiente;
		
		string nombrePersonaje; 	//Coincide con el nombre de la carpeta.		
		
		accion_posible nroAccionActual;
		Accion* accionActual;
		Estado* estado;
		
		bool mirarDerecha;
		
		LTexture* imagenActual;
		SDL_Renderer* renderer;
		
	//~ public:
		/* Recibe la coordenada de su extremo INFERIOR IZQUIERDO. */
		//Harcodeo para donde mira.
		Personaje(CoordenadaLogica* coord, string nombre, SDL_Renderer* ren, float ancho, float alto, Estado* estado, bool derecha, ConversorDeCoordenadas* conversor);
		//~ Después debería ser
		//~ Personaje::Personaje(CoordenadaLogica* coord, string nombre,SDL_Renderer* ren, float alto, float ancho, Estado* estado){
		ConversorDeCoordenadas* conversor;
		BarraDeVida* barraDeVida;
		Personaje(CoordenadaLogica* coordenada,BarraDeVida* barra, string nombre,SDL_Renderer* ren, float alto, float ancho, Estado* estado, Parser* conf);
		~Personaje();
		
		/* VIEJO:
		void cambiarAccionA(accion_posible nroAccion);
		void Dibujarse(float alto, float ancho);
		//void Dibujarse(int x, int y, int alto, int ancho);
		void Dibujarse(int x, int y, float alto, float ancho); // Esto esta sobrecargado *Manu*
		//~ int getSpriteActual();
		//Para Maxi
		//SDL_Texture* DibujarSpriteNumero(int numeroDeSprite);
		*/

		// Funciones internas
		void cambiarAccionA(accion_posible nroAccion);

		// Dibujarse que debería usarse finalmente así
		void Dibujarse();

        
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
