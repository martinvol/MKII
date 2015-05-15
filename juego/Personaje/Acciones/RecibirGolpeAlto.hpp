#ifndef RECIBIRGOLPEALTO_H_INCLUDED
#define RECIBIRGOLPEALTO_H_INCLUDED

#include "Accion.hpp"

class RecibirGolpeAlto:public Accion{
	public:
		RecibirGolpeAlto(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset);
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);	
		bool permiteAccion(accion_posible nuevaAccion);
		void cambiarModo();
};


#endif
