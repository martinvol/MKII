#include "PiniaBaja.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						PINIA
 * 
 **********************************************************************/  

PiniaBaja::PiniaBaja(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(7,ruta,ren,despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	this->porcentajeDeDanio = 2;
};

CoordenadaLogica* PiniaBaja::execute(CoordenadaLogica* coord_personaje){	
	
	Accion::cambiarModo();
	return new CoordenadaLogica(coord_personaje);
};

bool PiniaBaja::permiteAccion(accion_posible nuevaAccion){
	if(this->getModoActual() > 2){
		
		return true;
	}
	return false;
}		
