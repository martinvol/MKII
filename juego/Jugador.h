#ifndef JUGADOR_H
#define JUGADOR_H

#include "Personaje.h"
#include "BarraDeVida.h"

class Jugador {
	public:
		Personaje* personaje;
		BarraDeVida* barra;
		
		Personaje* obtenerPersonaje();
};

#endif //JUGADOR_H
