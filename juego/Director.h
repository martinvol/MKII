#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <vector>

#include "Personaje.h"
#include "Jugador.h"
#include "Escenario.h"
#include "CoordenadaLogica.h"
#include "CoordenadaFisica.h"
#include "ConversorDeCoordenadas.h"

using namespace std;

enum num_jugador {jugador1 = 0, jugador2};
enum movimiento {Nada, Derecha, Izquierda, Arriba};

class Director {
		
    public:
		vector<Personaje*> personajes;
		vector<Jugador*> jugadores;
		Escenario* escenario;
		ConversorDeCoordenadas* conversor;
		// Atributos necesarios para recopilar la información del loop y actualizar al final ambos jugadores.
		movimiento mov1 = Nada;
		movimiento mov2 = Nada;
		
		// NO PUEDEN USARSE:
		
		void verificar_movimientos();
		void verificar_orientaciones();
		
		
		
		
		// PUEDEN USARSE:
		
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
