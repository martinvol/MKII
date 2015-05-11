#ifndef QUIETO_H_INCLUDED
#define QUIETO_H_INCLUDED

#include "Accion.hpp"

class Quieto:public Accion{
	public:
		Quieto(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset);
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);	
		bool permiteAccion(accion_posible nuevaAccion);
};


#endif
