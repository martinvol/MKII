#ifndef ESTADO_H_INCLUDED
#define ESTADO_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include "../Acciones/Accion.hpp"
#include "../Acciones/Quieto.hpp"
#include "../Acciones/SaltarVertical.hpp"
#include "../Acciones/SaltarDiagonal.hpp"
#include "../Acciones/Caminar.hpp"

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
