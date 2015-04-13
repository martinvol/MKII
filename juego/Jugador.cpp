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
	personaje->mirarParaDerecha();
}

void Jugador::mirarParaIzquierda(){
	personaje->mirarParaIzquierda();
}

void Jugador::parar(){
	personaje->parar();
}

void Jugador::caminarDerecha(){
	personaje->caminarDerecha();
}

void Jugador::caminarIzquierda(){
	personaje->caminarIzquierda();
}

void Jugador::saltar(){
	personaje->saltar();
}

void Jugador::saltarDerecha(){
	personaje->saltarDerecha();
}

void Jugador::saltarIzquierda(){
	personaje->saltarIzquierda();
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
