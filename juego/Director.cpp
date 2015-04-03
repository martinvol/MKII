#include "Director.h"

void Director::seMuevePersonaje(num_jugador jugador, movimiento lugar){
	switch (jugador){
		case jugador1:
			this->mov1 = lugar;
			break;
		default:
			this->mov2 = lugar;
			break;
	}
}

void Director::actualizar(){
	verificar_movimientos();
	verificar_orientaciones();
}

void Director::verificar_orientaciones(){
	CoordenadaLogica* coord1 = (jugadores[jugador1]->obtenerPersonaje())->obtenerCoordenadaLogica();
	CoordenadaLogica* coord2 = (jugadores[jugador2]->obtenerPersonaje())->obtenerCoordenadaLogica();
	if (coord1->estaALaDerechaDe(coord2)){
		(jugadores[int(jugador1)]->obtenerPersonaje())->mirarParaIzquierda();
		(jugadores[int(jugador2)]->obtenerPersonaje())->mirarParaDerecha();
	} else {
		(jugadores[int(jugador1)]->obtenerPersonaje())->mirarParaDerecha();
		(jugadores[int(jugador2)]->obtenerPersonaje())->mirarParaIzquierda();
	}
}

