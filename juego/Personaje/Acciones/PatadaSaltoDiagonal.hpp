#ifndef PATADIAG_H_INCLUDED
#define PATADIAG_H_INCLUDED

#include "Accion.hpp"

class PatadaSaltoDiagonal:public Accion{
	public:
		PatadaSaltoDiagonal(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset);
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);	
		bool permiteAccion(accion_posible nuevaAccion);
		void cambiarModo();
		bool alcanzo_max;
		float dist_virtual;
};
#endif
