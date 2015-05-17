#include "PiniaAgachado.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						PINIA
 * 
 **********************************************************************/  

PiniaAgachado::PiniaAgachado(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(34,ruta,ren,despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
	rectangulos->push_back(new Rectangulo(0, 45, 14, 22, false, false)); // Cabeza
	rectangulos->push_back(new Rectangulo(10, 70, 10, 31, true, false)); // piña
	rectangulos->push_back(new Rectangulo(14, 20, 80, 50, false, false)); // piernas
	this->porcentajeDeDanio = 2;
};



void PiniaAgachado::cambiarModoInversamente(){
	if (this->modoActual==0){
		if(this->cantModos>0)
			setModoActual(this->cantModos-1);	
			ciclos = 1;
	}
	else{
		if (this->modoActual>0)
			setModoActual(this->modoActual-1);
	}
}	
  	
void PiniaAgachado::cambiarModo(){
	if (modoActual==(cantModos-2)){
		setModoActual(0);
		setInvertirSecuencia();	
	}else{
		setModoActual(this->modoActual+1);
	}
}


CoordenadaLogica* PiniaAgachado::execute(CoordenadaLogica* coord_personaje){	
	
	if(secuenciaInversa){
		PiniaAgachado::cambiarModoInversamente();	
	}
	else{
		PiniaAgachado::cambiarModo();
	}
	return new CoordenadaLogica(coord_personaje);
};

bool PiniaAgachado::permiteAccion(accion_posible nuevaAccion){
	if(this->getModoActual() > 2){
		
		return true;
	}
	return false;
}		
