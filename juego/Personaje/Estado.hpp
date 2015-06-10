		
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
#include "../Personaje/Acciones/RecibirGolpeAgachado.hpp"
#include "../Personaje/Acciones/Levantarse.hpp"
#include "../Personaje/Acciones/LevantarseDelGancho.hpp"
#include "../Personaje/Acciones/ArrojarProyectil.hpp"
#include "../Personaje/Acciones/Ganar.hpp"
#include "../Personaje/Acciones/Dizzy.hpp"
#include "../Personaje/Acciones/Morir.hpp"
#include "../Personaje/Acciones/Desaparecer.hpp"
#include "../Personaje/Acciones/Perform.hpp"

using namespace std;

class Estado{
	
	public:
	
		Quieto* quieto;
		Caminar* caminar;
		SaltarVertical* saltarvertical;
		SaltarDiagonal* saltardiagonal;
		Agacharse* agacharse;
		Girar* girar;
		
		/*******************REACCION*************************/

		RecibirTraba* recibirTraba;
		RecibirGolpeAlto* recibirgolpealto;
		RecibirGolpeBajo* recibirgolpebajo;
		RecibirGolpeAgachado* recibirGolpeAgachado;
		SiendoArrojado* siendoarrojado;
		Levantarse* levantarse;
		LevantarseDelGancho* levantarseDelGancho;

		/*******************ATAQUE*************************/

		///Pinias
		PiniaAlta* piniaAlta;
		PiniaBaja* piniaBaja;
		PiniaAgachado* piniaAgachado;
		PiniaAire* piniaAire;
		PiniaAire* piniaAireVertical;	//Vengo a flotar: Pregunta... son de la misma clase? what? Entonces para que existen dos?
		// Response: Una tiene una imagen menos, y no tiene velocidad en x.
		
		///Patadas
		PatadaAlta* patadaAlta;
		PatadaBaja* patadaBaja;
		PatadaBajaAgachado* patadaBajaAgachado;
		PatadaAltaAgachado* patadaAltaAgachado;
		PatadaSaltoDiagonal* patadaDiag;
		PatadaSaltoVertical* patadaVert;

		Traba* traba;
		Gancho* gancho;
		RoundKick* roundKick;
		
		ArrojarProyectil* arrojarproyectil;

		/*******************DEFENSA*************************/

		///Cubrir
		CubrirAlto* cubrirAlto;
		CubrirBajo* cubrirBajo;
		

		Ganar* ganar;
		Morir* morir;
		Dizzy* dizzy;
		Desaparecer* desaparecer;
		
		Perform* desenmascararse;
		
		Estado(string ruta, SDL_Renderer* ren, float alto_personaje, float alto_escenario, float ancho_personaje, float ancho_escenario, float ventana_ancho, int hue_init = 0, int hue_fin = 0, int hue_offset = 0);
		~Estado();
		
		string ruta_arrojable;

};

#endif
