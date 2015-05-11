#ifndef GIRAR_H_INCLUDED
#define GIRAR_H_INCLUDED

#include "Accion.hpp"

class Girar: public Accion{
	public:
		Girar(string ruta, SDL_Renderer* ren, SDL_Window* win, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset);
		void cambiarModoInversamente();
		void cambiarModo();
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);
		bool permiteAccion(accion_posible nuevaAccion);
};

#endif
