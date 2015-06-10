#ifndef GANAR_H_INCLUDED
#define GANAR_H_INCLUDED

#include "Accion.hpp"

class Ganar:public Accion{
	public:
		Ganar(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset);
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);	
		bool permiteAccion(accion_posible nuevaAccion);
		void iniciarSonido();
};


#endif
