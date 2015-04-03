#include "Jugador.h"

Personaje* Jugador::obtenerPersonaje(){
	return this->personaje;
}

CoordenadaLogica* obtenerCoordenada(){
	return (this->personaje)->obtenerCoordenada();
}

void Jugador::mirarParaDerecha(){
	(this->personaje)->mirarParaDerecha();
}

void Jugador::mirarParaIzquierda(){
	(this->personaje)->mirarParaIzquierda();
}
