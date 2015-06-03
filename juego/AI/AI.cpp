#include "AI.hpp"
#include "../Coordenadas/CoordenadaLogica.hpp"
#include <cmath>

AI::AI(Personaje* personajeAI, Personaje* personajeHumano) {
	
	this->personajeAI = personajeAI;
	this->personajeHumano = personajeHumano;
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


void AI::reaccionar(){
// Esta funcion en primera instancia, modificara los flags internos
// del joystick del personaje.

// Flags disponibles: 

//	Nada, Izquierda, Derecha, Arriba, Abajo;
//	PiniaAlta, PiniaBaja;
//	PatadaAlta, PatadaBaja;
//	ArrojarArma, CubrirAlto;

// 	Para poder usarlo, setear la constante USAR_AI en true en el main ! 
	
	this->personajeAI->CubrirAlto = true;	
}
