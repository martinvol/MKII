#ifndef PINIAAIRE_H_INCLUDED
#define PINIAAIRE_H_INCLUDED

#include "Accion.hpp"

class PiniaAire:public Accion{
	public:
		PiniaAire(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset);
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);	
		bool permiteAccion(accion_posible nuevaAccion);
		void cambiarModoInversamente();
		void cambiarModo();
		float dist_virtual;
		bool alcanzo_max;
};


#endif
