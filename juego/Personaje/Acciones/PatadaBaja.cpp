#include "PatadaBaja.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						PATADA BAJA
 * 
 **********************************************************************/ 

PatadaBaja::PatadaBaja(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(12,ruta,ren,despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
	rectangulos->push_back(new Rectangulo(0, 50, 14, 22, false, false)); // Cabeza
	rectangulos->push_back(new Rectangulo(0, 73, 15, 30, true, false)); // piña
	rectangulos->push_back(new Rectangulo(20, 20, 80, 50, false, false)); // piernas

	this->porcentajeDeDanio = 3;
};

CoordenadaLogica* PatadaBaja::execute(CoordenadaLogica* coord_personaje){	
	//~ cout<<"Modo a la entrada: "<<modoActual<<endl;
	Accion::cambiarModo();
	//~ cout<<"Modo a la salida: "<<modoActual<<endl;
	return new CoordenadaLogica(coord_personaje);
};

bool PatadaBaja::permiteAccion(accion_posible nuevaAccion){
	return false;
}		
