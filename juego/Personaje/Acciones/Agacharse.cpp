#include "Agacharse.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						AGACHARSE
 * 
 **********************************************************************/  

CoordenadaLogica* Agacharse::execute(CoordenadaLogica* coord_personaje){	
	return coord_personaje;
};

bool Agacharse::permiteAccion(accion_posible nuevaAccion){
	return false;
}
