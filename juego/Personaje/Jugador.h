#ifndef JUGADOR_H
#define JUGADOR_H

#include "Personaje.hpp"
#include "../Escenario/BarraDeVida.h"
#include "../Coordenadas/CoordenadaLogica.h"

class Jugador {
	public:
		Personaje* personaje;
		BarraDeVida* barra;
		
		Personaje* obtenerPersonaje();
		CoordenadaLogica* obtenerCoordenada();
		float obtenerAnchoLogico();
		/* Cambia orientación. */
		void mirarParaDerecha();
		void mirarParaIzquierda();
		/* Activa las imágenes.
		 * Y devuelve a dónde se movería. */
		CoordenadaLogica* parar();
		CoordenadaLogica* saltar();
		CoordenadaLogica* saltarDerecha();
		CoordenadaLogica* saltarIzquierda();
		CoordenadaLogica* caminarDerecha();
		CoordenadaLogica* caminarIzquierda();
		
};

#endif //JUGADOR_H
