#include "Quieto.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						QUIETO
 * 
 **********************************************************************/  

CoordenadaLogica* Quieto::execute(CoordenadaLogica* coord_personaje){
	Accion::cambiarModo();
	return new CoordenadaLogica(coord_personaje);
};

bool Quieto::permiteAccion(accion_posible nuevaAccion){
	return true;
}
