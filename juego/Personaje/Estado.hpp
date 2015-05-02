#ifndef ESTADO_H_INCLUDED
#define ESTADO_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include "../Personaje/Acciones/Accion.hpp"
#include "../Personaje/Acciones/Quieto.hpp"
#include "../Personaje/Acciones/SaltarVertical.hpp"
#include "../Personaje/Acciones/SaltarDiagonal.hpp"
#include "../Personaje/Acciones/Caminar.hpp"
#include "../Personaje/Acciones/Agacharse.hpp"

using namespace std;

class Estado{
	
	public:
	
		Quieto* quieto;
		Caminar* caminar;
		SaltarVertical* saltarvertical;
		SaltarDiagonal* saltardiagonal;
		Agacharse* agacharse;
		Estado(string ruta, SDL_Renderer* ren, float alto_personaje, float alto_escenario, float ancho_personaje, float ancho_escenario, float ventana_ancho);
		~Estado();

};

#endif
