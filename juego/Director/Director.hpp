#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <vector>

#include "../Personaje/Personaje.hpp"
#include "../Personaje/Jugador.hpp"
#include "../Escenario/Escenario.hpp"
#include "../Escenario/Ventana.hpp"
#include "../Escenario/Timer.hpp"
#include "../Coordenadas/CoordenadaLogica.hpp"
#include "../Coordenadas/CoordenadaFisica.hpp"
#include "../Coordenadas/ConversorDeCoordenadas.hpp"

using namespace std;

enum num_jugador {jugador1 = 0, jugador2};
enum movimiento {Nada, Derecha, Izquierda, Arriba, Abajo, ArribaDerecha, ArribaIzquierda, AbajoDerecha, AbajoIzquierda, PiniaAlta, PiniaBaja, MirarDerecha, MirarIzquierda};

class Director {
		
    private:
		//~ vector<Personaje*> personajes;
		vector<Jugador*> jugadores;
		Escenario* escenario;
		Ventana* ventana;
		Timer* timer;
		ConversorDeCoordenadas* conversor;
		
		float factor_scroll;
		
		// Atributos necesarios para recopilar la información del loop y actualizar al final ambos jugadores.
		movimiento mov1 = Nada;
		movimiento mov2 = Nada;
		
	
		void informar_acciones();
		void informar_accion(movimiento mov, Jugador* jugador);
		void verificar_movimientos();
		void verificar_movimiento(Jugador* jugador, Jugador* elOtro);
		void verificar_orientaciones();
		bool sePuedeScrollearDerecha();
		bool sePuedeScrollearIzquierda();
		void scrollearDerecha();
		void scrollearIzquierda();
		
	public:
		Director(Escenario* escenario, Ventana* ventana, ConversorDeCoordenadas* conversor, Personaje* personaje1, Personaje* personaje2, BarraDeVida* barra1, BarraDeVida* barra2, float factor_scroll, Timer* timer);
		~Director();
		
		/* Va a recibir donde intenta moverse, qué intenta hacer. */
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
