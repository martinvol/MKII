#include "CaminarDerecha.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						CAMINAR
 * 
 **********************************************************************/  
void CaminarDerecha::cambiarModoInversamente(){
	if (this->modoActual==0){
		if(this->cantModos>0)
			setModoActual(this->cantModos-1);	
	}
	else{
		if (this->modoActual>0)
			setModoActual(this->modoActual-1);
	}

}
void CaminarDerecha::execute(){
	if(secuenciaInversa){
		CaminarDerecha::cambiarModoInversamente();
	}
	else{
		Accion::cambiarModo();
	}
};
