#include "SaltarVertical.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						SALTAR VERTICALMENTE
 *
 **********************************************************************/  

void Saltar::resetear(){
	this->modoActual = 0;
	this->contadorDeLoops=0;
	this->contador = 0;
};

void Saltar::cambiarModo(){
	if (esUltimoModo()){
		if(this->cantModos>=3){///puts("entre aca");
			setModoActual(2);	
		}else
			setModoActual(0);
	}else{
		setModoActual(this->modoActual+1);
	}
};

void Saltar::execute(){
	if (this->getModoActual() == 1){
		if(contadorDeLoops<21){
			contadorDeLoops+=1;
			return;
		}
	}
	Saltar::cambiarModo();
};
