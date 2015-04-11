#ifndef PERSONAJE_H_INCLUDED
#define PERSONAJE_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "Accion.hpp"
#include "CoordenadaLogica.hpp"

using namespace std;

class Personaje {
		
		/* Coordenada Logica de la esquina izquierda superior. */	
		CoordenadaLogica* coordenada;
		/* En unidades logicas. */
		float alto;
		float ancho;
	
		string nombrePersonaje; 	//Coincide con el nombre de la carpeta.		
		Accion* accionActual;
		int lastTime;
		SDL_Texture* imagenActual;
		SDL_Renderer* renderer;
		void Dibujarse(int x, int y, int alto, int ancho);
		
	public:
		
		Personaje(CoordenadaLogica* coord, string nombre,SDL_Renderer* ren, float ancho_logico, float alto_logico);
		~Personaje();
		SDL_Texture* definir_imagen(int accion);
		void cambiar_posicion(int cant_pasos_x,int cant_pasos_y);
		void mirar_al_otro_lado();
		void cambiarAccionA(int nroAccion,string ruta, bool permiteInterrupcion);
		void Dibujarse(int x, int y);
		
		/* Sólo espejan o no espejan la imagen. */
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
		
		// COMENTARIO: No sé qué coordenadas voy a usar de estas y no sé
		// cuáles te convienen a vos. Esto se puede negociar.
		// De todas formas sabiendo una y el ancho y el alto lógicos
		// del personaje podrías calcularme todas y no me parece mal que
		// guardes tu ancho y alto lógico.
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
};

#endif
