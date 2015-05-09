#ifndef GIRAR_H_INCLUDED
#define GIRAR_H_INCLUDED

#include "Accion.hpp"

class Girar: public Accion{
	public:
		Girar(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max);
		void cambiarModoInversamente();
		void cambiarModo();
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);
		bool permiteAccion(accion_posible nuevaAccion);
};

#endif
