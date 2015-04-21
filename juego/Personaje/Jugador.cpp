#include "Jugador.hpp"

Jugador::Jugador(Personaje* personaje, BarraDeVida* barra){
	this->personaje = personaje;
	this->barra = barra;
}

Jugador::~Jugador(){
	delete personaje;
	delete barra;
}

void Jugador::mirarParaDerecha(){
	//~ personaje->mirarParaDerecha();
}

void Jugador::mirarParaIzquierda(){
	//~ personaje->mirarParaIzquierda();
}

void Jugador::activarAccion(accion_posible accion){
	personaje->activarAccion(accion);
}

CoordenadaLogica* Jugador::obtenerCoordenadaIzqSup(){
	return personaje->obtenerCoordenadaIzqSup();
}

CoordenadaLogica* Jugador::obtenerCoordenadaIzqInf(){
	return personaje->obtenerCoordenadaIzqInf();
}

CoordenadaLogica* Jugador::obtenerCoordenadaDerSup(){
	return personaje->obtenerCoordenadaDerSup();
}

CoordenadaLogica* Jugador::obtenerCoordenadaDerInf(){
	return personaje->obtenerCoordenadaDerInf();
}

CoordenadaLogica* Jugador::obtenerSiguienteCoordenadaIzqSup(){
	return personaje->obtenerSiguienteCoordenadaIzqSup();
}

CoordenadaLogica* Jugador::obtenerSiguienteCoordenadaIzqInf(){
	return personaje->obtenerSiguienteCoordenadaIzqInf();
}

CoordenadaLogica* Jugador::obtenerSiguienteCoordenadaDerSup(){
	return personaje->obtenerSiguienteCoordenadaDerSup();
}

CoordenadaLogica* Jugador::obtenerSiguienteCoordenadaDerInf(){
	return personaje->obtenerSiguienteCoordenadaDerInf();
}

void Jugador::moverseAIzqSup(CoordenadaLogica* coord){
	personaje->moverseAIzqSup(coord);
}

void Jugador::moverseADerSup(CoordenadaLogica* coord){
	personaje->moverseADerSup(coord);
}
