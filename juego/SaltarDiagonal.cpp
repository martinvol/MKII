#include "SaltarDiagonal.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						SALTAR DIAGONALMENTE
 *
 **********************************************************************/  

void SaltarDiagonal::cambiarModo(){
	if (esUltimoModo()){
		if(this->cantModos>=2)
			setModoActual(1);
		else
			setModoActual(0);
	}else{
		setModoActual(this->modoActual+1);
	}
}

void SaltarDiagonal::cambiarModoInversamente(){
	if(this->cantModos >= 2){
		if (this->modoActual==1){
			setModoActual(this->cantModos-1);	
		}
		else{
			setModoActual(this->modoActual-1);
		}
	}
}

void SaltarDiagonal::execute(){
	if(secuenciaInversa){
		SaltarDiagonal::cambiarModoInversamente();
	}
	else{
		SaltarDiagonal::cambiarModo();
	}
}

bool SaltarDiagonal::permiteAccion(accion_posible nuevaAccion){
	return false;
}
