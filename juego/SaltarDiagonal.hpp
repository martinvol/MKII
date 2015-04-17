#ifndef SALTARDIAGONAL_H_INCLUDED
#define SALTARDIAGONAL_H_INCLUDED

#include "Accion.hpp"

class SaltarDiagonal: public Accion{
	public:
		float dist_virtual;
		SaltarDiagonal(string ruta, SDL_Renderer* ren, float despl_x, float despl_y):Accion(4,ruta,ren, despl_x, despl_y){dist_virtual = 0;};	
		
		void cambiarModo();
		void cambiarModoInversamente();
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);
		bool permiteAccion(accion_posible nuevaAccion);
		void resetear();
		
};

#endif
