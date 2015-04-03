#ifndef VENTANA_H
#define VENTANA_H

#include "CoordenadaFisica.h"

enum lugarEnVentana {bordeIzq, medio, bordeDer};

class Ventana {

	public:
		
		SDL_Window * window = NULL;
		
		// Anchos físicos que identifican el final del borde izquierdo
		// y el inicio del borde derecho.
		int borde_izq, borde_der;
		
		// Margen identifica cuánto porcentaje de la ventana no es borde.		
		Ventana(char* titulo, int ancho_fisico, int alto_fisico, float margen);
		~Ventana();
		
		void cambiarTamaño(int ancho_fisico, int alto_fisico);
		void cambiarMargen(float margen);
		lugarEnVentana coordenadaEnPantalla(CoordenadaFisica* coord);
		
		

	
}

#endif //VENTANA_H
