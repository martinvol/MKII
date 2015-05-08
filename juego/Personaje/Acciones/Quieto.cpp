#include "Quieto.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						QUIETO
 * 
 **********************************************************************/  

Quieto::Quieto(string ruta, SDL_Renderer* ren, SDL_Window* win, float despl_x, float despl_y, float h_max):Accion(0,ruta,ren, win, despl_x, despl_y, h_max){
	cout <<"quieto "<< this->rectangulos->size() << '\n';
	Rectangulo* todo  = new Rectangulo(0,0,100,100,true);
	rectangulos->push_back(todo);
};

CoordenadaLogica* Quieto::execute(CoordenadaLogica* coord_personaje){
	Accion::cambiarModo();
	return new CoordenadaLogica(coord_personaje);
};

bool Quieto::permiteAccion(accion_posible nuevaAccion){
	return true;
}
