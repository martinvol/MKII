#ifndef SALTARVERTICAL_H_INCLUDED
#define SALTARVERTICAL_H_INCLUDED

#include "Accion.hpp"

class Saltar:public Accion{
	public:
		int contadorDeLoops;
		int contador;
		
		Saltar(string ruta, SDL_Renderer* ren):Accion(3,ruta,ren){this->contadorDeLoops=0;contador= 0;};
		
		void resetear();
		void cambiarModo();
		void execute();
};

#endif
