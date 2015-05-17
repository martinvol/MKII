#include "LevantarseDelGancho.hpp"

using namespace std;

/***********************************************************************
 * 
 * 						LevantarseDelGancho
 * 
 **********************************************************************/  

LevantarseDelGancho::LevantarseDelGancho(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(37,ruta,ren, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
	rectangulos->push_back(new Rectangulo(0, 50, 14, 22, false, false)); // Cabeza
	rectangulos->push_back(new Rectangulo(20, 0, 30, 100, false, false)); // torso
	rectangulos->push_back(new Rectangulo(49, 20, 50, 70, false, false)); // piernas
	intervalo = 350/cantModos;
}
///ESTO HACE QUE SOLO LO HAGA UNA VEZ NO IMPORTA QUE TANTO APRIETEN EL BOTON!
void LevantarseDelGancho::cambiarModo(){
	if(esUltimoModo()){
		ciclos = 1;
	}
	else{
		setModoActual(this->modoActual+1);
	}
}

CoordenadaLogica* LevantarseDelGancho::execute(CoordenadaLogica* coord_personaje){
	float taux = SDL_GetTicks();
	if (taux - tinicial > intervalo){
		LevantarseDelGancho::cambiarModo();
		tinicial = taux;
	}
	return new CoordenadaLogica(coord_personaje);
}

bool LevantarseDelGancho::permiteAccion(accion_posible nuevaAccion){
	return false;
}

