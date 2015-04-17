#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <vector>

#include "Personaje.hpp"
#include "Jugador.hpp"
#include "Escenario.hpp"
#include "Ventana.hpp"
#include "CoordenadaLogica.hpp"
#include "CoordenadaFisica.hpp"
#include "ConversorDeCoordenadas.hpp"


// Cuánto ancho lógico se mueve al scrollear.
#define FACTOR_SCROLL 5

using namespace std;

enum num_jugador {jugador1 = 0, jugador2};
enum movimiento {Nada, Derecha, Izquierda, Arriba, ArribaDerecha, ArribaIzquierda};

class Director {
		
    private:
		//~ vector<Personaje*> personajes;
		vector<Jugador*> jugadores;
		Escenario* escenario;
		Ventana* ventana;
		ConversorDeCoordenadas* conversor;
		// Atributos necesarios para recopilar la información del loop y actualizar al final ambos jugadores.
		movimiento mov1 = Nada;
		movimiento mov2 = Nada;
		
	
		void analizar_multievento_de_un_jugador(movimiento* mov, movimiento lugar);
		void informar_acciones();
		void informar_accion(movimiento mov, Jugador* jugador);
		void verificar_movimientos();
		void verificar_orientaciones();
		bool sePuedeScrollearDerecha();
		bool sePuedeScrollearIzquierda();
		void scrollearDerecha();
		void scrollearIzquierda();
		
	public:
		Director(Escenario* escenario, Ventana* ventana, ConversorDeCoordenadas* conversor, Personaje* personaje1, Personaje* personaje2, BarraDeVida* barra1, BarraDeVida* barra2);
		~Director();
		
		/* Sólo va a recibir Derecha, Izquierda, Arriba o Nada. */
		void seMuevePersonaje(num_jugador jugador, movimiento lugar);
		
		/* Si un personaje se mueve, debe validar:
		 * que si se va de margen, scrollee la ventana.
		 * que no se pase del escenario.
		 * que no se mueva si ambos jugadores están en el borde y el
		 *     movimiento del otro no se lo permite.
		 * Debe definir para dónde deben mirar considerando el nuevo 
		 * lugar de los personajes.
		*/
		void actualizar();
		
};

#endif //DIRECTOR_H
