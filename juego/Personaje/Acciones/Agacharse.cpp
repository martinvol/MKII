#include "Agacharse.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						AGACHARSE
 * 
 **********************************************************************/  

void Agacharse::cambiarModo(){
	if (esUltimoModo()){
	}else{
		setModoActual(this->modoActual+1);
	}
}

CoordenadaLogica* Agacharse::execute(CoordenadaLogica* coord_personaje){	
	
	Agacharse::cambiarModo();
	return new CoordenadaLogica(coord_personaje);
};

bool Agacharse::permiteAccion(accion_posible nuevaAccion){
	return false;
}

