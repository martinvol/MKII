#include "AI.hpp"

AI::AI(Personaje* personajeAI, Personaje* personajeHumano) {
	
	this->personajeAI = personajeAI;
	this->personajeHumano = personajeHumano;
}

void AI::reaccionar(){
	this->personajeAI->Abajo = true;
}
