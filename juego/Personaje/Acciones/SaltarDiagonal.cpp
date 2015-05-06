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

CoordenadaLogica* SaltarDiagonal::execute(CoordenadaLogica* coord_personaje){
	dist_virtual += despl_y;
	CoordenadaLogica* coord = new CoordenadaLogica(coord_personaje);
	// Cambio de coordenada.
	if (dist_virtual < h_max) coord->desplazarY(despl_y);
	else coord->desplazarY(-despl_y);
	if (dist_virtual >= 2*h_max) return coord;
	// Cambio de imagen.
	// ANTES:	//~ if(secuenciaInversa){
		//~ SaltarDiagonal::cambiarModoInversamente();
	//~ }
	//~ else{
		//~ SaltarDiagonal::cambiarModo();
	//~ }
	// AHORA:
	if (impar){
		// Si es el modo del medio que se mantiene
		if (secuenciaInversa){
			if ((cantModos+1)/2 >= (modoActual+1)){
				if (dist_virtual >= delta*(cantModos+1 -modoActual)){
					cambiarModoInversamente();
				}
				coord->desplazarX(-despl_x);
				return coord;
			}
			coord->desplazarX(-despl_x);
		} else {
			if ((cantModos+1)/2 <= (modoActual+1)){
				if (dist_virtual >= delta*(modoActual+2)){
					SaltarDiagonal::cambiarModo();	
				}
				coord->desplazarX(despl_x);
				return coord;
			}
			coord->desplazarX(despl_x);
		}
	}
	// Si es impar y está en la primera mitad, o si es par.
	if (secuenciaInversa){
		if (dist_virtual >= delta*(cantModos-modoActual)){
			cambiarModoInversamente();
		}
		coord->desplazarX(-despl_x);
	}else{
		
		if (dist_virtual >= delta*(modoActual)){
			SaltarDiagonal::cambiarModo();
		}
		coord->desplazarX(despl_x);
	}
	return coord;
}

bool SaltarDiagonal::permiteAccion(accion_posible nuevaAccion){
	return false;
}

void SaltarDiagonal::resetear(){
	Accion::resetear();
	dist_virtual = 0;
}
