#include "RecibirTraba.hpp"

using namespace std;

/***********************************************************************
 * 
 * 						RecibirTraba
 * 
 **********************************************************************/  

RecibirTraba::RecibirTraba(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(35,ruta,ren, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
	rectangulos->push_back(new Rectangulo(0, 50, 14, 22, false, false)); // Cabeza
	rectangulos->push_back(new Rectangulo(20, 0, 30, 100, false, false)); // torso
	rectangulos->push_back(new Rectangulo(49, 20, 50, 70, false, false)); // piernas
	iniciarSonido();
}
///ESTO HACE QUE SOLO LO HAGA UNA VEZ NO IMPORTA QUE TANTO APRIETEN EL BOTON!
void RecibirTraba::cambiarModo(){
	if(esUltimoModo()){}
	else{
		Accion::cambiarModo();
	}
}

void RecibirTraba::iniciarSonido(){
	sonido = Mix_LoadWAV("resources/music/punch.wav");
}

CoordenadaLogica* RecibirTraba::execute(CoordenadaLogica* coord_personaje){
	Accion::cambiarModo();
	return new CoordenadaLogica(coord_personaje);
}

bool RecibirTraba::permiteAccion(accion_posible nuevaAccion){
	return false;
}

