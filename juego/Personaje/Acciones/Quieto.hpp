#ifndef QUIETO_H_INCLUDED
#define QUIETO_H_INCLUDED

#include "Accion.hpp"

class Quieto:public Accion{
	public:
		Quieto(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max);
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);	
		bool permiteAccion(accion_posible nuevaAccion);
};


#endif
