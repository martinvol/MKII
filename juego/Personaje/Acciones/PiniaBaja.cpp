#include "PiniaBaja.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						PINIA
 * 
 **********************************************************************/  
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
