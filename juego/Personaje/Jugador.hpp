#ifndef JUGADOR_H
#define JUGADOR_H

#include "Personaje.hpp"
#include "../Escenario/BarraDeVida.hpp"
#include "../Coordenadas/CoordenadaLogica.hpp"

class Jugador {
	public:
		Personaje* personaje;
		BarraDeVida* barra;
		
		Jugador(Personaje* personaje, BarraDeVida* barra);
		~Jugador();
		
	// Para PERSONAJE:
	
		// Si se necesita
		Personaje* obtenerPersonaje();
		
		/* Sólo espejan o no espejan la imagen. */
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
		 * acción que fue pedida de "activar". */
		 /* Devuelven coordenadas que deben ser liberadas para no perder
		 * memoria. */
		CoordenadaLogica* obtenerSiguienteCoordenadaIzqSup();
		CoordenadaLogica* obtenerSiguienteCoordenadaIzqInf();
		CoordenadaLogica* obtenerSiguienteCoordenadaDerSup();
		CoordenadaLogica* obtenerSiguienteCoordenadaDerInf();
	
		/* Coordenada a la que debe moverse el pesonaje. */
		void moverseAIzqSup(CoordenadaLogica* coord);
		void moverseADerSup(CoordenadaLogica* coord);
	
	
	// Para BARRA DE VIDA:
		
};

#endif //JUGADOR_H
