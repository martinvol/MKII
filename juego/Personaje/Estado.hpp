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
#include "../Personaje/Acciones/PiniaAlta.hpp"
#include "../Personaje/Acciones/PiniaBaja.hpp"
#include "../Personaje/Acciones/Girar.hpp"

using namespace std;

class Estado{
	
	public:
	
		Quieto* quieto;
		Caminar* caminar;
		SaltarVertical* saltarvertical;
		SaltarDiagonal* saltardiagonal;
		Agacharse* agacharse;
		Estado(string ruta, SDL_Renderer* ren, float alto_personaje, float alto_escenario, float ancho_personaje, float ancho_escenario, float ventana_ancho, int hue_init = 0, int hue_fin = 0, int hue_offset = 0);
		PiniaAlta* piniaAlta;
		PiniaBaja* piniaBaja;
		Girar* girar;
		~Estado();

};

#endif
