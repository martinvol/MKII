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

void SaltarVertical::execute(){
	if (this->getModoActual() == 1){
		if(contadorDeLoops<21){
			contadorDeLoops+=1;
			return;
		}
	}
	SaltarVertical::cambiarModo();
};

bool SaltarVertical::permiteAccion(accion_posible nuevaAccion){
	return false;
}
