#ifndef VENTANA_H
#define VENTANA_H

#include "CoordenadaFisica.h"
#include "ConversorDeCoordenadas.h"

enum lugarEnVentana {bordeIzq, medio, bordeDer};

class Ventana {

	public:
		
		SDL_Window * window = NULL;
		
		// Anchos físicos que identifican el final del borde izquierdo
		// y el inicio del borde derecho.
		int borde_izq, borde_der;
		int ancho;
		
		
		// Margen identifica cuánto porcentaje de la ventana no es borde.		
		Ventana(char* titulo, int ancho_fisico, int alto_fisico, float margen);
		~Ventana();
		
		void cambiarTamano(int ancho_fisico, int alto_fisico);
		void cambiarMargen(float margen);
		/*Se supone que no recibe una coordenada física que supere a la pantalla nunca. (No lo valida) */
		lugarEnVentana coordenadaEnPantalla(CoordenadaFisica* coord);
		bool superaTecho(CoordenadaFisica* coord);
		/* Devuelven coordenadas que deben liberarse. */
		float obtenerMargenLogicoIzquierdo(ConversorDeCoordenadas* conv);
		float obtenerMargenLogicoDerecho(ConversorDeCoordenadas* conv);	
		float obtenerBordeLogicoIzquierdo(ConversorDeCoordenadas* conv);
		float obtenerBordeLogicoDerecho(ConversorDeCoordenadas* conv);
		float obtenerBordeSuperior(ConversorDeCoordenadas* conv);	
};

#endif //VENTANA_H
