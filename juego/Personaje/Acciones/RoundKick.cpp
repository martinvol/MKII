#include "RoundKick.hpp"

using namespace std;

/***********************************************************************
 * 
 * 						RoundKick
 * 
 **********************************************************************/  

RoundKick::RoundKick(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(ROUNDKICK,ruta,ren, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
	
	rectangulos->push_back(new Rectangulo(0, 40, 14, 22, false, false)); // Cabeza
	rectangulos->push_back(new Rectangulo(20, 0, 30, 70, false, false)); // torso
	rectangulos->push_back(new Rectangulo(70, 20, 50, 70, true, false)); // piernas
};


///ESTO HACE QUE SOLO LO HAGA UNA VEZ NO IMPORTA QUE TANTO APRIETEN EL BOTON!
//~ void RoundKick::cambiarModo(){
	//~ if(esUltimoModo()){}
	//~ else{
		//~ Accion::cambiarModo();
	//~ }
//~ }

CoordenadaLogica* RoundKick::execute(CoordenadaLogica* coord_personaje){
	Accion::cambiarModo();
	return new CoordenadaLogica(coord_personaje);
};

bool RoundKick::permiteAccion(accion_posible nuevaAccion){
	return false;
}
