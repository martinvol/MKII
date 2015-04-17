#include "Quieto.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						QUIETO
 * 
 **********************************************************************/  

void Quieto::execute(){
	Accion::cambiarModo();
};

bool Quieto::permiteAccion(accion_posible nuevaAccion){
	return true;
}
