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
void Caminar::execute(){
	if(secuenciaInversa){
		Caminar::cambiarModoInversamente();
	}
	else{
		Accion::cambiarModo();
	}
};

bool Caminar::permiteAccion(accion_posible nuevaAccion){
	return true;
}
