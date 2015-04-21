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
		if(this->cantModos>=3){
			setModoActual(2);	
		}else
			setModoActual(0);
	}else{
		setModoActual(this->modoActual+1);
	}
};

CoordenadaLogica* SaltarVertical::execute(CoordenadaLogica* coord_personaje){
	dist_virtual += despl_y;
	CoordenadaLogica* coord = new CoordenadaLogica(coord_personaje);
	// Cambio de coordenada.
	coord->desplazarX(despl_x);
	if (dist_virtual < h_max) coord->desplazarY(despl_y);
	else coord->desplazarY(-despl_y);
	if (dist_virtual >= 2*h_max) return coord;
	// Cambio de imagen.
	//ANTES:
	//~ if (modoActual == 1){
		//~ if(contadorDeLoops<21){
			//~ contadorDeLoops+=1;
			//~ return coord;
		//~ }
	//~ }
	//~ SaltarVertical::cambiarModo();
	//~ return coord;
	//AHORA:
	if (impar){
		// Si es el modo del medio que se mantiene
		if ((cantModos+1)/2 <= (modoActual+1)){
			if (dist_virtual >= delta*(modoActual+2)){
				cambiarModo();
			}
			return coord;
		}
	}
	// Si es impar y está en la primera mitad, o si es par.
	if (dist_virtual >= delta*(modoActual+1)) cambiarModo(); // Que se supone que para esta implementación sólo va a avanzar al siguiente.
	return coord;
};

bool SaltarVertical::permiteAccion(accion_posible nuevaAccion){
	return true; // return false; ?
}
