#include "Jugador.hpp"

/******************************************************************
 *
 *					CONSTRUCTOR Y DESTRUCTOR
 * 
 ******************************************************************/

Jugador::Jugador(Personaje* personaje, BarraDeVida* barra){
	this->personaje = personaje;
	this->barra = barra;	
}

Jugador::~Jugador(){
	delete personaje;
	delete barra;
}


/******************************************************************
 *
 *						para PERSONAJE
 * 
 ******************************************************************/

Personaje* Jugador::obtenerPersonaje(){
	return this->personaje;
}

void Jugador::mirarParaDerecha(){
	(this->personaje)->mirarParaDerecha();
}

void Jugador::mirarParaIzquierda(){
	(this->personaje)->mirarParaIzquierda();
}

void Jugador::activarAccion(accion_posible accion){
	(this->personaje)->activarAccion(accion);
}

CoordenadaLogica* Jugador::obtenerCoordenadaIzqSup(){
	return (this->personaje)->obtenerCoordenadaIzqSup();
}

CoordenadaLogica* Jugador::obtenerCoordenadaIzqInf(){
	return (this->personaje)->obtenerCoordenadaIzqInf();
}

CoordenadaLogica* Jugador::obtenerCoordenadaDerSup(){
	return (this->personaje)->obtenerCoordenadaDerSup();
}

CoordenadaLogica* Jugador::obtenerCoordenadaDerInf(){
	return (this->personaje)->obtenerCoordenadaDerInf();
}

CoordenadaLogica* Jugador::obtenerSiguienteCoordenadaIzqSup(){
	return (this->personaje)->obtenerSiguienteCoordenadaIzqSup();
}

CoordenadaLogica* Jugador::obtenerSiguienteCoordenadaIzqInf(){
	return (this->personaje)->obtenerSiguienteCoordenadaIzqInf();
}

CoordenadaLogica* Jugador::obtenerSiguienteCoordenadaDerSup(){
	return (this->personaje)->obtenerSiguienteCoordenadaDerSup();
}

CoordenadaLogica* Jugador::obtenerSiguienteCoordenadaDerInf(){
	return (this->personaje)->obtenerSiguienteCoordenadaDerInf();
}

void Jugador::moverseAIzqSup(CoordenadaLogica* coord){
	(this->personaje)->moverseAIzqSup(coord);
}

void Jugador::moverseADerSup(CoordenadaLogica* coord){
	(this->personaje)->moverseADerSup(coord);
}
