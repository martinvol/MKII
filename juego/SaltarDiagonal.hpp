#ifndef SALTARDIAGONAL_H_INCLUDED
#define SALTARDIAGONAL_H_INCLUDED

#include "Accion.hpp"

class SaltarDiagonal: public Accion{
	public:
		SaltarDiagonal(string ruta, SDL_Renderer* ren):Accion(4,ruta,ren){};	
		
		void cambiarModo();
		void cambiarModoInversamente();
		void execute();
		
};

#endif
