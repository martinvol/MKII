#include "CubrirAlto.hpp"

using namespace std;

/***********************************************************************
 * 
 * 						CUBRIRALTO
 * 
 **********************************************************************/  

CubrirAlto::CubrirAlto(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(9,ruta,ren, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
	rectangulos->push_back(new Rectangulo(0, 50, 14, 22, false, false)); // Cabeza
	rectangulos->push_back(new Rectangulo(20, 0, 30, 100, false, false)); // torso
	rectangulos->push_back(new Rectangulo(49, 20, 50, 70, false, false)); // piernas
};

void CubrirAlto::cambiarModo(){
	if(esUltimoModo()){}
	else{
		Accion::cambiarModo();
	}
}

CoordenadaLogica* CubrirAlto::execute(CoordenadaLogica* coord_personaje){
	CubrirAlto::cambiarModo();
	return new CoordenadaLogica(coord_personaje);
};

bool CubrirAlto::permiteAccion(accion_posible nuevaAccion){
	return true;
}
