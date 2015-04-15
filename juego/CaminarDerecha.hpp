#ifndef CAMINARDERECHA_H_INCLUDED
#define CAMINARDERECHA_H_INCLUDED

#include "Accion.hpp"

class CaminarDerecha: public Accion{
	public:
		CaminarDerecha(string ruta, SDL_Renderer* ren):Accion(1,ruta,ren){};
		void cambiarModoInversamente();
		void execute();
		
};

#endif
