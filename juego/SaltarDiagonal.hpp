#ifndef SALTARDIAGONAL_H_INCLUDED
#define SALTARDIAGONAL_H_INCLUDED

#include "Accion.hpp"

class SaltarDiagonal: public Accion{
	public:
		SaltarDiagonal(string ruta, SDL_Renderer* ren, float despl_x, float despl_y):Accion(4,ruta,ren, despl_x, despl_y){};	
		
		void cambiarModo();
		void cambiarModoInversamente();
		void execute();
		bool permiteAccion(accion_posible nuevaAccion);
		
};

#endif
