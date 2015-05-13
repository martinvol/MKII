#ifndef ROUNDKICK_H_INCLUDED
#define ROUNDKICK_H_INCLUDED

#include "Accion.hpp"

class RoundKick:public Accion{
	public:
		RoundKick(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset);
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);	
		bool permiteAccion(accion_posible nuevaAccion);
		void cambiarModo();
};


#endif
