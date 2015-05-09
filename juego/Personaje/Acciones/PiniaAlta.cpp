#include "PiniaAlta.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						PINIA
 * 
 **********************************************************************/  
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
