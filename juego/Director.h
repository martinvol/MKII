#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <vector>

#include "Personaje.hpp"
#include "Jugador.h"
#include "Escenario.h"
#include "Ventana.h"
#include "CoordenadaLogica.h"
#include "CoordenadaFisica.h"
#include "ConversorDeCoordenadas.h"

using namespace std;

enum num_jugador {jugador1 = 0, jugador2};
enum movimiento {Nada, Derecha, Izquierda, Arriba, ArribaDerecha, ArribaIzquierda};

class Director {
		
    public:
		vector<Personaje*> personajes;
		vector<Jugador*> jugadores;
		Escenario* escenario;
		Ventana* ventana;
		ConversorDeCoordenadas* conversor;
		// Atributos necesarios para recopilar la información del loop y actualizar al final ambos jugadores.
		movimiento mov1 = Nada;
		movimiento mov2 = Nada;
		bool scrolleo1 = false;
		bool scrolleo2 = false;
		CoordenadaLogica* nueva_coord1 = NULL;
		CoordenadaLogica* nueva_coord2 = NULL;
		
		// NO PUEDEN USARSE:
		
		void verificar_movimiento_y_scroll();
		void verificar_orientaciones();
		void verificar_necesidad_de_scroll_y_accionar_movimientos();
		
		
		
		// PUEDEN USARSE:
		/* Sólo va a recibir Derecha, Izquierda o Arriba. */
		void seMuevePersonaje(num_jugador jugador, movimiento lugar);
		
		
		
		/* Si un personaje se mueve, debe validar:
		 * que si se va de margen, scrollee la ventana.
		 * que no se pase del escenario.
		 * que no se mueva si ambos jugadores están en el borde y el
		 *     movimiento del otro no se lo permite.
		*/
		/* Debe definir para dónde deben mirar considerando el nuevo 
		 * lugar de los personajes.
		*/
		void actualizar();
		
};

#endif //DIRECTOR_H
