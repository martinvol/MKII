#ifndef CAMINARDERECHA_H_INCLUDED
#define CAMINARDERECHA_H_INCLUDED

#include "Accion.hpp"

class Caminar: public Accion{
	public:
		Caminar(string ruta, SDL_Renderer* ren, SDL_Window* win, float despl_x, float despl_y, float h_max):Accion(1,ruta,ren, win, despl_x, despl_y, h_max){};
		void cambiarModoInversamente();
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);
		bool permiteAccion(accion_posible nuevaAccion);
};

#endif
