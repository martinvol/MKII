#ifndef ESTADO_H_INCLUDED
#define ESTADO_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include "Accion.hpp"
#include "Quieto.hpp"
#include "SaltarVertical.hpp"
#include "SaltarDiagonal.hpp"
#include "CaminarDerecha.hpp"


class Estado{
	
	public:
	
		Quieto* quieto;
		CaminarDerecha* caminarder;
		Saltar* saltar;
		SaltarDiagonal* saltardiagonal;
		

		Estado(string ruta, SDL_Renderer* ren){
			this->quieto = new Quieto(ruta, ren);
			this->saltar = new Saltar(ruta, ren);
			this->saltardiagonal = new SaltarDiagonal(ruta, ren);
			this->caminarder = new CaminarDerecha(ruta, ren);
		};
		~Estado(){
			delete this->quieto;
			delete this->caminarder;
			delete this->saltar;
			delete this->saltardiagonal;
		}

};

#endif
