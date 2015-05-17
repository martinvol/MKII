#include "PatadaSaltoDiagonal.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						Pinia en el aire
 * 
 **********************************************************************/  

PatadaSaltoDiagonal::PatadaSaltoDiagonal(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(PATADASALTANDODIAGONAL,ruta,ren, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
	rectangulos->push_back(new Rectangulo(0, 0, 70, 65, false, false)); // Cabeza
	rectangulos->push_back(new Rectangulo(70, 50, 15, 35, true, false)); // piña
	rectangulos->push_back(new Rectangulo(85, 70, 15, 30, true, false)); // piña
}

void PatadaSaltoDiagonal::cambiarModo(){
	
	if (! esUltimoModo()) setModoActual(this->modoActual+1);
}

CoordenadaLogica* PatadaSaltoDiagonal::execute(CoordenadaLogica* coord_personaje){
	dist_virtual = coord_personaje->y;
	if (dist_virtual >= h_max) this->alcanzo_max = true;
	if (dist_virtual <= 0) this->alcanzo_max = false;
	this->cambiarModo();
	CoordenadaLogica* coord = new CoordenadaLogica(coord_personaje);
	float signo = direccionDerecha? 1:-1;
	coord->desplazarX(despl_x*signo);
	// Cambio de coordenada.
	if (this->alcanzo_max) coord->desplazarY(-despl_y);
	else coord->desplazarY(despl_y);
	return coord;
};

bool PatadaSaltoDiagonal::permiteAccion(accion_posible nuevaAccion){
	return false;
}

