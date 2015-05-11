#include "Caminar.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						CAMINAR
 * 
 **********************************************************************/


Caminar::Caminar(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(1,ruta,ren, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	rectangulos->push_back(new Rectangulo(0, 50, 14, 22, false, false)); // Cabeza
	rectangulos->push_back(new Rectangulo(20, 0, 30, 100, false, false)); // torso
	rectangulos->push_back(new Rectangulo(49, 20, 50, 70, false, false)); // piernas
}

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

CoordenadaLogica* Caminar::execute(CoordenadaLogica* coord_personaje){
	CoordenadaLogica* coord = new CoordenadaLogica(coord_personaje);
	coord->desplazarY(despl_y);
	if (secuenciaInversa){
		Caminar::cambiarModoInversamente();
	} else {
		Accion::cambiarModo();
	}
	if (direccionDerecha){
		coord->desplazarX(despl_x);
	} else {
		coord->desplazarX(-despl_x);
	}
	return coord;
};

bool Caminar::permiteAccion(accion_posible nuevaAccion){
	return true;
}
