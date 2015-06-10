#ifndef FATALITY_H_INCLUDED
#define FATALITY_H_INCLUDED

#include "../Personaje.hpp"
#include "Perform.hpp"

class Fatality {
	Personaje* victima;
	Personaje* victimario;
	Perform* perform;
	public:
		void execute();
};

#endif
