#ifndef PERFORM_H_INCLUDED
#define PERFORM_H_INCLUDED

#include "Accion.hpp"

class Perform:public Accion{
	public:
		Perform(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset);
		bool permiteAccion(accion_posible nuevaAccion);
};


#endif
