#ifndef DirectorPractica_H
#define DirectorPractica_H

#include <vector>

#include "Director.hpp"
#include "../Personaje/Personaje.hpp"
#include "../Personaje/Jugador.hpp"
#include "../Escenario/Escenario.hpp"
#include "../Escenario/Ventana.hpp"
#include "../Escenario/Timer.hpp"
#include "../Coordenadas/CoordenadaLogica.hpp"
#include "../Coordenadas/CoordenadaFisica.hpp"
#include "../Coordenadas/ConversorDeCoordenadas.hpp"

using namespace std;

class DirectorPractica: public Director {
		
    private:
		vector<Jugador*> jugadores;
		Escenario* escenario=NULL;
		Ventana* ventana;
		Timer* timer;
		ConversorDeCoordenadas* conversor;
		float y_piso;
		
		// Atributos necesarios para recopilar la información del loop y actualizar al final ambos jugadores.
		movimiento mov1 = Nada;
		movimiento mov2 = Nada;
		
		void verificar_movimientos();
		void verificar_movimiento(Jugador* jugador, Jugador* elOtro);
		void verificar_orientaciones();
		bool sePuedeScrollearDerecha();
		bool sePuedeScrollearIzquierda();
		void scrollearDerecha(float factor_scroll);
		void scrollearIzquierda(float factor_scroll);
		
	public:
		DirectorPractica(Escenario* escenario, Ventana* ventana, ConversorDeCoordenadas* conversor, float y_piso, Personaje* personaje1, Personaje* personaje2, BarraDeVida* barra1, BarraDeVida* barra2, Timer* timer);
		//~DirectorPractica();
		
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
		bool seMurio(int num_jugador);
		
};

#endif //DirectorPractica_H
