#include "PatadaSaltoDiagonal.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						Pinia en el aire
 * 
 **********************************************************************/  

PatadaSaltoDiagonal::PatadaSaltoDiagonal(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(5,ruta,ren, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
	//rectangulos->push_back(new Rectangulo(0, 45, 14, 22, false, false)); // Cabeza
	//rectangulos->push_back(new Rectangulo(20, 10, 15, 70, false, false)); // torso
	//rectangulos->push_back(new Rectangulo(35, 10, 17, 90, false, false)); // torso
	//rectangulos->push_back(new Rectangulo(49, 15, 50, 70, false, false)); // piernas
}

void PatadaSaltoDiagonal::cambiarModo(){
	
	if (! esUltimoModo()) setModoActual(this->modoActual+1);
}

void PatadaSaltoDiagonal::cambiarModoInversamente(){
	if (this->modoActual > 0){
		setModoActual(this->modoActual-1);
	}
}

CoordenadaLogica* PatadaSaltoDiagonal::execute(CoordenadaLogica* coord_personaje){
	CoordenadaLogica* coord = new CoordenadaLogica(coord_personaje);
	if(!secuenciaInversa){
		PatadaSaltoDiagonal::cambiarModo();
	}else{
		PatadaSaltoDiagonal::cambiarModoInversamente();
	}
	return coord;
};

bool PatadaSaltoDiagonal::permiteAccion(accion_posible nuevaAccion){
	return false;
}

