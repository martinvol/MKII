#include "SaltarVertical.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						SALTAR VERTICALMENTE
 *
 **********************************************************************/  

void SaltarVertical::resetear(){
	this->modoActual = 0;
	this->contadorDeLoops=0;
	this->contador = 0;
	this->dist_virtual = 0;
};

void SaltarVertical::cambiarModo(){
	if (esUltimoModo()){
		if(this->cantModos>=3){///puts("entre aca");
			setModoActual(2);	
		}else
			setModoActual(0);
	}else{
		setModoActual(this->modoActual+1);
	}
};

CoordenadaLogica* SaltarVertical::execute(CoordenadaLogica* coord_personaje){
	CoordenadaLogica* coord = new CoordenadaLogica(coord_personaje);
	coord->desplazarX(despl_x);
	if (dist_virtual < H_MAX) coord->desplazarY(despl_y);
	else coord->desplazarY(-despl_y);
	if (this->getModoActual() == 1){
		if(contadorDeLoops<21){
			contadorDeLoops+=1;
			return coord;
		}
	}
	SaltarVertical::cambiarModo();
	return coord;
};

bool SaltarVertical::permiteAccion(accion_posible nuevaAccion){
	return false;
}
