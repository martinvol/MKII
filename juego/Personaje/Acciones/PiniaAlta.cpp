#include "PiniaAlta.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						PINIA
 * 
 **********************************************************************/ 

PiniaAlta::PiniaAlta(string ruta, SDL_Renderer* ren, SDL_Window* win, float despl_x, float despl_y, float h_max):Accion(6,ruta,ren, win, despl_x, despl_y, h_max){
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
	rectangulos->push_back(new Rectangulo(0, 50, 14, 22, false, false)); // Cabeza
	rectangulos->push_back(new Rectangulo(0, 73, 30, 30, true, false)); // piña
	rectangulos->push_back(new Rectangulo(20, 20, 80, 50, false, false)); // piernas

	this->porcentajeDeDanio = 3;
};

CoordenadaLogica* PiniaAlta::execute(CoordenadaLogica* coord_personaje){	
	
	Accion::cambiarModo();
	return new CoordenadaLogica(coord_personaje);
};

bool PiniaAlta::permiteAccion(accion_posible nuevaAccion){
	if(this->getModoActual() > 2){
		
		return true;
	}
	return false;
}		
