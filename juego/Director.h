#ifndef DIRECTOR_H
#define DIRECTOR_H

#include "Personaje.h"
#include "CoordenadaLogica.h"

//enum num_personaje {personaje1, personaje2};

class Director {
		
    public:
		void seMuevePersonajeA(Personaje personaje, CoordenadaLogica coord); // o Física?
		// o void seMuevePersonajeA(num_personaje personaje, CoordenadaLogica coord);
};

#endif //DIRECTOR_H
