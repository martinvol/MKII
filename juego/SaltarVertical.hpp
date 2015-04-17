#ifndef SALTARVERTICAL_H_INCLUDED
#define SALTARVERTICAL_H_INCLUDED

#include "Accion.hpp"

class SaltarVertical:public Accion{
	public:
		int contadorDeLoops;
		int contador;
		float dist_virtual;
		
		SaltarVertical(string ruta, SDL_Renderer* ren, float despl_x, float despl_y):Accion(3,ruta,ren, despl_x, despl_y){this->contadorDeLoops=0;contador= 0;dist_virtual=0;};
		
		void resetear();
		void cambiarModo();
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);
		bool permiteAccion(accion_posible nuevaAccion);
};

#endif
