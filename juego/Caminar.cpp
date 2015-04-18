#include "Caminar.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						CAMINAR
 * 
 **********************************************************************/  
void Caminar::cambiarModoInversamente(){
	if (this->modoActual==0){
		if(this->cantModos>0)
			setModoActual(this->cantModos-1);	
	}
	else{
		if (this->modoActual>0)
			setModoActual(this->modoActual-1);
	}

}

CoordenadaLogica* Caminar::execute(CoordenadaLogica* coord_personaje){
	CoordenadaLogica* coord = new CoordenadaLogica(coord_personaje);
	coord->desplazarY(despl_y);
	if(secuenciaInversa){
		Caminar::cambiarModoInversamente();
		coord->desplazarX(-despl_x);
		return coord;
	}
	// else
	Accion::cambiarModo();
	coord->desplazarX(despl_x);
}

bool Caminar::permiteAccion(accion_posible nuevaAccion){
	return true;
}
