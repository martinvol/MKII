#include "AI.hpp"
#include "../Coordenadas/CoordenadaLogica.hpp"
#include <cmath>
#include <random>
#include <cstdlib>
#include <ctime>

#define PROBA_REACCION 0.76

#define RG1 this->personajeAI->ancho/2;
#define RG2 this->personajeAI->ancho*2;

typedef enum {CLOSE, MID, FAR} Rg;

AI::AI(Personaje* personajeAI, Personaje* personajeHumano) {
	
	this->personajeAI = personajeAI;
	this->personajeHumano = personajeHumano;
	srand (static_cast <unsigned> (time(0)));
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
	float dist = dist(p1, p2);
	if (dist <= RG1) return CLOSE;
	if (dist <= RG2 && dist > RG1) return MID;
	return FAR;
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
	
	if (ran01() > PROBA_REACCION) return;
	
	
}
