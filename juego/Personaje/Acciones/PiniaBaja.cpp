#include "PiniaBaja.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						PINIA
 * 
 **********************************************************************/  

PiniaBaja::PiniaBaja(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(7,ruta,ren,despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
	rectangulos->push_back(new Rectangulo(0, 50, 14, 22, false, false)); // Cabeza
	rectangulos->push_back(new Rectangulo(14, 73, 10, 28, true, false)); // piña
	rectangulos->push_back(new Rectangulo(20, 20, 80, 50, false, false)); // piernas
	
	iniciarSonido();	
};

void PiniaBaja::iniciarSonido(){
	sonido = Mix_LoadWAV("resources/music/male_scream_short.wav");
}

void PiniaBaja::cambiarModoInversamente(){
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
  	
void PiniaBaja::cambiarModo(){
	if (modoActual==(cantModos-2)){
		setModoActual(0);
		setInvertirSecuencia();	
	}else{
		setModoActual(this->modoActual+1);
	}
}


CoordenadaLogica* PiniaBaja::execute(CoordenadaLogica* coord_personaje){	
	
	if(secuenciaInversa){
		PiniaBaja::cambiarModoInversamente();	
	}
	else{
		PiniaBaja::cambiarModo();
	}
	return new CoordenadaLogica(coord_personaje);
};

bool PiniaBaja::permiteAccion(accion_posible nuevaAccion){
	if(this->getModoActual() > 2){
		
		return true;
	}
	return false;
}		
