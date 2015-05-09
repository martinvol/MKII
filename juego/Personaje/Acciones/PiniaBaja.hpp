#ifndef PINIABAJA_H_INCLUDED
#define PINIABAJA_H_INCLUDED


#include "Accion.hpp"

class PiniaBaja: public Accion{
	float porcentajeDeDanio;
	public:
		PiniaBaja(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max):Accion(7,ruta,ren, despl_x, despl_y, h_max){
			this->porcentajeDeDanio = 2;
		};
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);
		bool permiteAccion(accion_posible nuevaAccion);		
};

#endif
