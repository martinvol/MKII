#ifndef ESTADO_H_INCLUDED
#define ESTADO_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include "Accion.hpp"

class Estado{
	
	public:
	
		Quieto* quieto;
		CaminarDerecha* caminarder;
		CaminarIzquierda* caminarizq;
		Saltar* saltar;
		SaltarDiagonal* saltardiagonal;

		Estado(string ruta, SDL_Renderer* ren){
			quieto = new Quieto(ruta, ren);
			saltar = new Saltar(ruta, ren);
			saltardiagonal = new SaltarDiagonal(ruta, ren);
			caminarizq = new CaminarIzquierda(ruta, ren);
			caminarder = new CaminarDerecha(ruta, ren);
		};

};

#endif
