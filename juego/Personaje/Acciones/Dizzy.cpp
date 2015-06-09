#include "Dizzy.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						DIZZY
 * 
 **********************************************************************/  

Dizzy::Dizzy(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(42,ruta,ren, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
	
	rectangulos->push_back(new Rectangulo(0, 50, 14, 22, false, false)); // Cabeza
	rectangulos->push_back(new Rectangulo(14, 0, 21, 80, false, false)); // torso
	rectangulos->push_back(new Rectangulo(35, 0, 15, 100, false, false)); // torso
	rectangulos->push_back(new Rectangulo(49, 20, 50, 70, false, false)); // piernas
};

CoordenadaLogica* Dizzy::execute(CoordenadaLogica* coord_personaje){
	Accion::cambiarModo();
	return new CoordenadaLogica(coord_personaje);
};

bool Dizzy::permiteAccion(accion_posible nuevaAccion){
	return true;
}
