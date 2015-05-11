#ifndef SALTARDIAGONAL_H_INCLUDED
#define SALTARDIAGONAL_H_INCLUDED

#include "Accion.hpp"

class SaltarDiagonal: public Accion{
	public:
		float delta;
		float dist_virtual;
		bool impar;
		
		SaltarDiagonal(string ruta, SDL_Renderer* ren, SDL_Window* win, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(4,ruta,ren, win, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
			dist_virtual = 0;
			if (cantModos%2==1){
				// cantidad impar de imágenes.
				delta = 2.0 * h_max / (cantModos+1);
				impar = true;
			} else {
				delta = 2.0 * h_max/cantModos;
				impar = false;
			}
			rectangulos->push_back(new Rectangulo(20, 0, 30, 100, false, false)); // torso
			rectangulos->push_back(new Rectangulo(49, 20, 30, 70, false, false)); // piernas
		}
		
		void cambiarModo();
		void cambiarModoInversamente();
		CoordenadaLogica* execute(CoordenadaLogica* coord_personaje);
		bool permiteAccion(accion_posible nuevaAccion);
		void resetear();
		
};

#endif
