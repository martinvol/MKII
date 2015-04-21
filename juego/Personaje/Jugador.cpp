#include "Jugador.hpp"

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

CoordenadaLogica* Jugador::parar(){
	(this->personaje)->parar();
}

CoordenadaLogica* Jugador::saltar(){
	(this->personaje)->saltar();
}

CoordenadaLogica* Jugador::saltarDerecha(){
	(this->personaje)->saltarDerecha();
}

CoordenadaLogica* Jugador::saltarIzquierda(){
	(this->personaje)->saltarIzquierda();
}

CoordenadaLogica* Jugador::caminarDerecha(){
	(this->personaje)->caminarDerecha();
}

CoordenadaLogica* Jugador::caminarIzquierda(){
	(this->personaje)->caminarIzquierda();
}
