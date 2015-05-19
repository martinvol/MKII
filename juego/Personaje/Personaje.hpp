#ifndef PERSONAJE_H_INCLUDED
#define PERSONAJE_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "Estado.hpp"
#include "../Personaje/Acciones/Accion.hpp"
#include "../Personaje/Acciones/Arrojable.hpp"
#include "../Coordenadas/CoordenadaLogica.hpp"
#include "../Coordenadas/ConversorDeCoordenadas.hpp"
#include "../Parser/Parser.hpp"
#include "../Escenario/BarraDeVida.hpp"
#include "../Logger/Logger.hpp"

#define JOYSTICK_DEAD_ZONE 10000


using namespace std;
//enum botones {Nada, Izquierda, Derecha, Arriba, Abajo, PiniaAlta, PiniaBaja, MirarDerecha, MirarIzquierda};

class Personaje {
		
	public:
	bool Nada, Izquierda, Derecha, Arriba, Abajo, PiniaAlta, PiniaBaja, PatadaAlta, PatadaBaja, ArrojarArma, CubrirAlto;
		
	void ActualizarControlador(SDL_Joystick *joystick, Parser* conf, SDL_Event *evento);
	
	//~ private:
		/* Sus anchos y altos lógicos. */
		float ancho;
		float alto;
		
		/* Para escalar imagen */
		int _h, _w;
		int ancho_fisico, alto_fisico;
		int ancho_quieto, altura_quieto;
		int x, y, w, h;
		
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
		
		SDL_Texture* imagenActual;
		SDL_Renderer* renderer;

		Arrojable* arrojable = NULL;
		
	//~ public:
		/* Recibe la coordenada de su extremo INFERIOR IZQUIERDO. */
		//Harcodeo para donde mira.
		Personaje(CoordenadaLogica* coord, string nombre, SDL_Renderer* ren, float ancho, float alto, Estado* estado, ConversorDeCoordenadas* conversor, float velocidad_arma, int numeroJugador, bool miraADerecha);
		int numero_jugador;
		//~ Después debería ser
		//~ Personaje::Personaje(CoordenadaLogica* coord, string nombre,SDL_Renderer* ren, float alto, float ancho, Estado* estado){
		ConversorDeCoordenadas* conversor;
		BarraDeVida* barraDeVida;
		Personaje(CoordenadaLogica* coordenada,BarraDeVida* barra, string nombre,SDL_Renderer* ren, float alto, float ancho, Estado* estado, Parser* conf);
		~Personaje();
		void Arrojar();

		// Funciones internas
		void cambiarAccionA(accion_posible nroAccion);

		// Dibujarse que debería usarse finalmente así
		void Dibujarse();
		
		float calcularAnchoYAltoImagen();
		void calcularDatosDibujables();
		
		// Funciones usables:
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
		void moverseAIzqInf(CoordenadaLogica* coord);
		void moverseADerInf(CoordenadaLogica* coord);
		
	private:
		SDL_Texture* imagenArrojable;
		float velocidad_arma;
};



#endif
