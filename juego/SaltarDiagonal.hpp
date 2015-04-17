#ifndef SALTARDIAGONAL_H_INCLUDED
#define SALTARDIAGONAL_H_INCLUDED

#include "Accion.hpp"

class SaltarDiagonal: public Accion{
	public:
		float delta;
		float dist_virtual;
		bool impar;
		
		SaltarDiagonal(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max):Accion(4,ruta,ren, despl_x, despl_y, h_max){
			dist_virtual = 0;
			if (cantModos%2==1){
				// cantidad impar de imágenes.
				delta = 2.0 * h_max / (cantModos+1);
				impar = true;
			} else {
				delta = 2.0 * h_max/cantModos;
				impar = false;
			}	
		}	
		
		void cambiarModo();
		void cambiarModoInversamente();
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);
		bool permiteAccion(accion_posible nuevaAccion);
		void resetear();
		
};

#endif
