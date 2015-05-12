#include "Agacharse.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						AGACHARSE
 * 
 **********************************************************************/  

Agacharse::Agacharse (string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(5,ruta,ren, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	


}

void Agacharse::cambiarModo(){
	if (esUltimoModo()){
	}else{
		setModoActual(this->modoActual+1);
	}
}

void Agacharse::cambiarModoInversamente(){
	if (this->modoActual > 0){
		setModoActual(this->modoActual-1);
	}
}


//~ CoordenadaLogica* Caminar::execute(CoordenadaLogica* coord_personaje){
	//~ CoordenadaLogica* coord = new CoordenadaLogica(coord_personaje);
	//~ coord->desplazarY(despl_y);
	//~ if (secuenciaInversa){
		//~ Caminar::cambiarModoInversamente();
	//~ } else {
		//~ Accion::cambiarModo();
	//~ }
	//~ if (direccionDerecha){
		//~ coord->desplazarX(despl_x);
	//~ } else {
		//~ coord->desplazarX(-despl_x);
	//~ }
	//~ return coord;
//~ };


CoordenadaLogica* Agacharse::execute(CoordenadaLogica* coord_personaje){
	CoordenadaLogica* coord = new CoordenadaLogica(coord_personaje);
	//~ coord->desplazarX(despl_x);
	if(!secuenciaInversa){
		Agacharse::cambiarModo();
		//~ coord->desplazarY(-despl_y);
	}else{
		Agacharse::cambiarModoInversamente();
		//~ coord->desplazarY(despl_y);
	}
	return coord;
};

bool Agacharse::permiteAccion(accion_posible nuevaAccion){
	return false;
}

