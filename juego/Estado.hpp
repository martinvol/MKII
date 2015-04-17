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
#include "Caminar.hpp"

#define VELOCIDAD_VERTICAL 75
#define VELOCIDAD_HORIZONTAL 50

using namespace std;

class Estado{
	
	public:
	
		Quieto* quieto;
		Caminar* caminar;
		SaltarVertical* saltarvertical;
		SaltarDiagonal* saltardiagonal;
		

		Estado(string ruta, SDL_Renderer* ren, float alto_personaje, float alto_escenario, float ancho_personaje, float ancho_escenario);
		~Estado();

};

#endif
