#include "CubrirBajo.hpp"

using namespace std;

/***********************************************************************
 * 
 * 						CUBRIR BAJO
 * 
 **********************************************************************/  

CubrirBajo::CubrirBajo(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(10,ruta,ren, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
	rectangulos->push_back(new Rectangulo(0, 50, 14, 22, false, false)); // Cabeza
	rectangulos->push_back(new Rectangulo(20, 0, 30, 100, false, false)); // torso
	rectangulos->push_back(new Rectangulo(49, 20, 50, 70, false, false)); // piernas
};

void CubrirBajo::cambiarModo(){
	if(esUltimoModo()){
		ciclos = 1;
	}
	else{
		Accion::cambiarModo();
	}
}

CoordenadaLogica* CubrirBajo::execute(CoordenadaLogica* coord_personaje){
	puts ("CUbrir bajo de la accion mismoaaljflksjglkbjuslkhgjjn");
	CubrirBajo::cambiarModo();
	return new CoordenadaLogica(coord_personaje);
};

bool CubrirBajo::permiteAccion(accion_posible nuevaAccion){
	return false;
}
