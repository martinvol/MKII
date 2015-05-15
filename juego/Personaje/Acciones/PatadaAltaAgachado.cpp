#include "PatadaAltaAgachado.hpp"

using namespace std;

/***********************************************************************
 * 
 * 						PATADA ALTA AGACHADO
 * 
 **********************************************************************/  

PatadaAltaAgachado::PatadaAltaAgachado(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(14,ruta,ren, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
	rectangulos->push_back(new Rectangulo(4, 24, 14, 20, false, false)); // Cabeza
	rectangulos->push_back(new Rectangulo(0, 70, 14, 22, true, false)); // pata
	rectangulos->push_back(new Rectangulo(14, 50, 14, 20, true, false)); // pata
	rectangulos->push_back(new Rectangulo(20, 0, 30, 40, false, false)); // torso
	rectangulos->push_back(new Rectangulo(49, 10, 55, 40, false, false)); // piernas
};

void PatadaAltaAgachado::cambiarModoInversamente(){
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
  	
void PatadaAltaAgachado::cambiarModo(){
	if (modoActual==(cantModos-2)){
		setModoActual(0);
		setInvertirSecuencia();	
	}else{
		setModoActual(this->modoActual+1);
	}
}



CoordenadaLogica* PatadaAltaAgachado::execute(CoordenadaLogica* coord_personaje){
	Accion::cambiarModo();
	return new CoordenadaLogica(coord_personaje);
};

bool PatadaAltaAgachado::permiteAccion(accion_posible nuevaAccion){
	return true;
}
