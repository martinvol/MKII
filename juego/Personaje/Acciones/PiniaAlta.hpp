#ifndef PINIAALTA_H_INCLUDED
#define PINIAALTA_H_INCLUDED


#include "Accion.hpp"

class PiniaAlta: public Accion{
	float porcentajeDeDanio;
	public:
		PiniaAlta(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max):Accion(6,ruta,ren, despl_x, despl_y, h_max){
			this->porcentajeDeDanio = 3;
		};
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);
		bool permiteAccion(accion_posible nuevaAccion);		
};

#endif
