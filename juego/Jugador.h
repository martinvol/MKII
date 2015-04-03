#ifndef JUGADOR_H
#define JUGADOR_H

#include "Personaje.hpp"
#include "BarraDeVida.h"
#include "CoordenadaLogica.h"

class Jugador {
	public:
		Personaje* personaje;
		BarraDeVida* barra;
		
		Personaje* obtenerPersonaje();
		CoordenadaLogica* obtenerCoordenada();
		/* Cambia
		void mirarParaDerecha();
		void mirarParaIzquierda();
		/* Devuelve la coordenada en la que está. */
		CoordenadaLogica* parar();
		/* Devuelve la coordenada a la que iría. */
		CoordenadaLogica* saltar();
		/* Devuelve la coordenada a la que iría. */
		CoordenadaLogica* caminarDerecha();
		/* Devuelve la coordenada a la que iría. */
		CoordenadaLogica* caminarIzquierda();
		
};

#endif //JUGADOR_H
