#ifndef JUGADOR_H
#define JUGADOR_H

#include "Personaje.hpp"
#include "BarraDeVida.hpp"
#include "CoordenadaLogica.hpp"

class Jugador {
	public:
		Personaje* personaje;
		BarraDeVida* barra;
		
	// Para PERSONAJE:
		
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
