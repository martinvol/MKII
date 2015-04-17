#ifndef CAMINARDERECHA_H_INCLUDED
#define CAMINARDERECHA_H_INCLUDED

#include "Accion.hpp"

class Caminar: public Accion{
	public:
		Caminar(string ruta, SDL_Renderer* ren, float despl_x, float despl_y):Accion(1,ruta,ren, despl_x, despl_y){};
		void cambiarModoInversamente();
		void execute();
		bool permiteAccion(accion_posible nuevaAccion);		
};

#endif
