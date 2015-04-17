#ifndef SALTARVERTICAL_H_INCLUDED
#define SALTARVERTICAL_H_INCLUDED

#include "Accion.hpp"

class SaltarVertical:public Accion{
	public:
		int contadorDeLoops;
		int contador;
		
		SaltarVertical(string ruta, SDL_Renderer* ren, float despl_x, float despl_y):Accion(3,ruta,ren, despl_x, despl_y){this->contadorDeLoops=0;contador= 0;};
		
		void resetear();
		void cambiarModo();
		void execute();
		bool permiteAccion(accion_posible nuevaAccion);
};

#endif
