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

using namespace std;

class Estado{
	
	public:
	
		Quieto* quieto;
		CaminarDerecha* caminarder;
		Saltar* saltar;
		SaltarDiagonal* saltardiagonal;
		

		Estado(string ruta, SDL_Renderer* ren);
		~Estado();

};

#endif
