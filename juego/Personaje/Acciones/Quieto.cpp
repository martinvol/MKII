#include "Quieto.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						QUIETO
 * 
 **********************************************************************/  

Quieto::Quieto(string ruta, SDL_Renderer* ren, SDL_Window* win, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(0,ruta,ren, win, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	cout <<"quieto "<< this->rectangulos->size() << '\n';
	Rectangulo* todo  = new Rectangulo(0,0,100,100,true,false);
	rectangulos->push_back(todo);
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
	rectangulos->push_back(new Rectangulo(0, 50, 14, 22, false, false)); // Cabeza
	rectangulos->push_back(new Rectangulo(20, 0, 30, 100, false, false)); // torso
	rectangulos->push_back(new Rectangulo(49, 20, 50, 70, false, false)); // piernas
};

CoordenadaLogica* Quieto::execute(CoordenadaLogica* coord_personaje){
	Accion::cambiarModo();
	return new CoordenadaLogica(coord_personaje);
};

bool Quieto::permiteAccion(accion_posible nuevaAccion){
	return true;
}
