#include "PatadaAlta.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						PATADA ALTA
 * 
 **********************************************************************/ 

PatadaAlta::PatadaAlta(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(11,ruta,ren,despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
	rectangulos->push_back(new Rectangulo(0, 10, 14, 22, false, false)); // Cabeza
	rectangulos->push_back(new Rectangulo(15, 65, 15, 38, true, false)); // piña
	rectangulos->push_back(new Rectangulo(30, 35, 15, 38, true, false)); // piña
	rectangulos->push_back(new Rectangulo(20, 0, 80, 50, false, false)); // piernas

	this->porcentajeDeDanio = 3;
};

CoordenadaLogica* PatadaAlta::execute(CoordenadaLogica* coord_personaje){	
	//~ cout<<"Modo a la entrada: "<<modoActual<<endl;
	Accion::cambiarModo();
	//~ cout<<"Modo a la salida: "<<modoActual<<endl;
	return new CoordenadaLogica(coord_personaje);
};

bool PatadaAlta::permiteAccion(accion_posible nuevaAccion){
	return false;
}		
