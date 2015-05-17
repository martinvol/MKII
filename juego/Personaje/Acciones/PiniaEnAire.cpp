#include "PiniaEnAire.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						Pinia en el aire
 * 
 **********************************************************************/  

PiniaAire::PiniaAire(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(PINIASALTANDODIAGONAL,ruta,ren, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)

	rectangulos->push_back(new Rectangulo(0, 0, 100, 70, false, false)); // torso
	rectangulos->push_back(new Rectangulo(50, 70, 20, 35, true, false)); // piñá
}

void PiniaAire::cambiarModo(){
	// Saco una imagen por si es salto vertical
	if (despl_x == 0 && this->modoActual == 0) this->modoActual = 1;
	if (! esUltimoModo()) setModoActual(this->modoActual+1);
}

void PiniaAire::cambiarModoInversamente(){
	if (this->modoActual > 0){
		setModoActual(this->modoActual-1);
	}
}

CoordenadaLogica* PiniaAire::execute(CoordenadaLogica* coord_personaje){
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

bool PiniaAire::permiteAccion(accion_posible nuevaAccion){
	return false;
}

