#ifndef AGACHARSE_H_INCLUDED
#define AGACHARSE_H_INCLUDED

#include "Accion.hpp"

class Agacharse: public Accion{
	public:
		Agacharse(string ruta, SDL_Renderer* ren, SDL_Window* win, float despl_x, float despl_y, float h_max):Accion(5,ruta,ren, win, despl_x, despl_y, h_max){};
		void cambiarModo();
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);
		bool permiteAccion(accion_posible nuevaAccion);		
};

#endif
