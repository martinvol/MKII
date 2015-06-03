#include "AI.hpp"

AI::AI(Personaje* personajeAI, Personaje* personajeHumano) {
	
	this->personajeAI = personajeAI;
	this->personajeHumano = personajeHumano;
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
