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
enum movimiento {Nada, Derecha, Izquierda, Arriba, Abajo, 
				Traba, ArribaDerecha, ArribaIzquierda, AbajoDerecha,
				AbajoIzquierda, PiniaAlta, PiniaBaja, MirarDerecha, MirarIzquierda, Pararse, 
				CubrirAlto, CubrirBajo, ArrojarArma, PatadaAlta, PatadaBaja, PatadaBajaAgachado, 
				PatadaAltaAgachado, PatadaSaltoVertical, PiniaSaltandoVertical, 
				PiniaSaltandoDiagonal, PatadaSaltoDiagonal, Gancho, RoundKick};

class Director {
		
    private:
		vector<Jugador*> jugadores;
		Escenario* escenario;
		Ventana* ventana;
		Timer* timer;
		ConversorDeCoordenadas* conversor;
		float y_piso;
		
		// Atributos necesarios para recopilar la información del loop y actualizar al final ambos jugadores.
		movimiento mov1 = Nada;
		movimiento mov2 = Nada;
		
		virtual void verificar_movimientos();
		virtual void verificar_movimiento(Jugador* jugador, Jugador* elOtro);
		virtual void verificar_orientaciones();
		virtual bool sePuedeScrollearDerecha();
		virtual bool sePuedeScrollearIzquierda();
		virtual void scrollearDerecha(float factor_scroll);
		virtual void scrollearIzquierda(float factor_scroll);
		
	public:
		Director();
		Director(Escenario* escenario, Ventana* ventana, ConversorDeCoordenadas* conversor, float y_piso, Personaje* personaje1, Personaje* personaje2, BarraDeVida* barra1, BarraDeVida* barra2, Timer* timer);
		virtual ~Director();
		
		/* Va a recibir donde intenta moverse, qué intenta hacer. */
		virtual void seMuevePersonaje(num_jugador jugador, movimiento lugar);
		
		/* Si un personaje se mueve, debe validar:
		 * que si se va de margen, scrollee la ventana.
		 * que no se pase del escenario.
		 * que no se mueva si ambos jugadores están en el borde y el
		 *     movimiento del otro no se lo permite.
		 * Debe definir para dónde deben mirar considerando el nuevo 
		 * lugar de los personajes.
		*/
		virtual void actualizar();
		virtual bool seMurio(int num_jugador);
		
};

bool IntersectRect(const SDL_Rect * r1, const SDL_Rect * r2);
void presentarAnimacionRecibirGolpe(Personaje* pegando, Personaje* sufre);
#endif //DIRECTOR_H
