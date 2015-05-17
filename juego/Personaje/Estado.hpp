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
#include "../Personaje/Acciones/PiniaAgachado.hpp"
#include "../Personaje/Acciones/PiniaBaja.hpp"
#include "../Personaje/Acciones/Girar.hpp"
#include "../Personaje/Acciones/CubrirAlto.hpp"
#include "../Personaje/Acciones/CubrirBajo.hpp"
#include "../Personaje/Acciones/PatadaAlta.hpp"
#include "../Personaje/Acciones/PatadaBaja.hpp"
#include "../Personaje/Acciones/PatadaBajaAgachado.hpp"
#include "../Personaje/Acciones/PatadaAltaAgachado.hpp"
#include "../Personaje/Acciones/Traba.hpp"
#include "../Personaje/Acciones/PiniaEnAire.hpp"
#include "../Personaje/Acciones/PatadaSaltoDiagonal.hpp"
#include "../Personaje/Acciones/PatadaSaltoVertical.hpp"
#include "../Personaje/Acciones/RoundKick.hpp"
#include "../Personaje/Acciones/Gancho.hpp"
#include "../Personaje/Acciones/RecibirGolpeBajo.hpp"
#include "../Personaje/Acciones/RecibirGolpeAlto.hpp"
#include "../Personaje/Acciones/SiendoArrojado.hpp"
#include "../Personaje/Acciones/RecibirTraba.hpp"

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
		PiniaAgachado* piniaAgachado;
		PatadaAlta* patadaAlta;
		PatadaBaja* patadaBaja;
		PatadaBajaAgachado* patadaBajaAgachado;
		PatadaAltaAgachado* patadaAltaAgachado;
		Girar* girar;
		Traba* traba;
		CubrirAlto* cubrirAlto;
		CubrirBajo* cubrirBajo;
		Gancho* gancho;
		RecibirTraba* recibirTraba;
		RoundKick* roundKick;
		string ruta_arrojable;
		~Estado();
		
		// Acciones Aereas
		PiniaAire* piniaAire;
		PatadaSaltoDiagonal* patadaDiag;
		PatadaSaltoVertical* patadaVert;
		PiniaAire* piniaAireVertical;
		
		// Recibir Golpes
		RecibirGolpeAlto* recibirgolpealto;
		RecibirGolpeBajo* recibirgolpebajo;
		SiendoArrojado* siendoarrojado;
};

#endif
