#ifndef FATALITY_H_INCLUDED
#define FATALITY_H_INCLUDED

#include "../Personaje.hpp"
#include "Perform.hpp"

class Fatality {
	
	public:
		Personaje* victima;
		Personaje* victimario;
		Perform* acido;
		Perform* desenmascararse;
		void execute();
};

#endif
