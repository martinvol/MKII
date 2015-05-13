#include "PatadaBajaAgachado.hpp"

using namespace std;

/***********************************************************************
 * 
 * 						PATADA BAJA AGACHADO
 * 
 **********************************************************************/  

PatadaBajaAgachado::PatadaBajaAgachado(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(13,ruta,ren, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
	rectangulos->push_back(new Rectangulo(0, 30, 14, 22, false, false)); // Cabeza
	rectangulos->push_back(new Rectangulo(20, 0, 30, 50, false, false)); // torso
	rectangulos->push_back(new Rectangulo(49, 10, 50, 40, false, false)); // piernas
	rectangulos->push_back(new Rectangulo(49, 20, 50, 75, true, false)); // piernas
};

CoordenadaLogica* PatadaBajaAgachado::execute(CoordenadaLogica* coord_personaje){
	Accion::cambiarModo();
	return new CoordenadaLogica(coord_personaje);
};

bool PatadaBajaAgachado::permiteAccion(accion_posible nuevaAccion){
	return true;
}
