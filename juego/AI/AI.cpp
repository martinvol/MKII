#include "AI.hpp"
#include "../Coordenadas/CoordenadaLogica.hpp"
#include <cmath>
#include <random>
#include <cstdlib>
#include <ctime>

#define PROBA_REACCION 0.76

#define RG1 p1->ancho/2
#define RG2 p1->ancho*2

Rg RgDist(Personaje* p1, Personaje* p2);

AI::AI(Personaje* personajeAI, Personaje* personajeHumano) {
	
	this->personajeAI = personajeAI;
	this->personajeHumano = personajeHumano;
	srand (static_cast <unsigned> (time(0)));
	this->where = RgDist(personajeAI, personajeHumano); ///
}

float ran01() {
	return ((float) rand()) / (float) RAND_MAX;
}

bool left(Personaje* p) {
	return !(p->mirarDerecha);
}

float dist_c(CoordenadaLogica* c1, CoordenadaLogica* c2) {
	return sqrt((c1->x - c2->x)*(c1->x - c2->x) + (c1->y - c2->y)*(c1->y - c2->y));
}

float dist(Personaje* p1, Personaje* p2) {
	return (left(p1)? dist_c(p1->obtenerCoordenadaDerInf(), p2->obtenerCoordenadaIzqInf()):
	dist_c(p1->obtenerCoordenadaIzqInf(), p2->obtenerCoordenadaDerInf()));
}

Rg RgDist(Personaje* p1, Personaje* p2) {
	float d = dist(p1, p2);
	if (d <= RG1) return CLOSE;
	if ((d <= RG2) && (d > RG1)) return MID;
	return FAR;
}

void cleanController(Personaje* p) {
	p->Nada = p->Izquierda = p->Derecha = p->Arriba =
	p->Abajo = p->PiniaAlta = p->PiniaBaja = p->PatadaAlta =
	p->PatadaBaja = p->ArrojarArma = p->CubrirAlto = false;
}

bool AI::updateH(){
	if (accAntH != this->personajeHumano->nroAccionActual) {
		accAntH = this->personajeHumano->nroAccionActual;
		return true;
	}
	return false;
}

bool AI::updateS(){
	if (accAntS != this->personajeAI->nroAccionActual) {
		accAntS = this->personajeAI->nroAccionActual;
		if (this->personajeAI->nroAccionActual == CAMINAR_DERECHA || 
			this->personajeAI->nroAccionActual == CAMINAR_IZQUIERDA)
				cleanController(this->personajeAI);
		return true;
	}
	return false;
}

bool AI::updateWhere() {
	Rg range = RgDist(this->personajeHumano, this->personajeAI);
	
	if (this->where != range) {
		this->where = range;
		return true;
	}
	
	return false;
}

void AI::reaccionar(){
// Esta funcion en primera instancia, modificara los flags internos
// del joystick del personaje.

// Flags disponibles: 

//	Nada, Izquierda, Derecha, Arriba, Abajo;
//	PiniaAlta, PiniaBaja;
//	PatadaAlta, PatadaBaja;
//	ArrojarArma, CubrirAlto;

// 	Para poder usarlo, setear la constante USAR_AI en true en el main ! 
	
	//if (ran01() > PROBA_REACCION) return;
	
	if (!(this->updateH() && this->updateWhere())) return;
	
	this->personajeAI->CubrirAlto = !this->personajeAI->CubrirAlto;	
}
