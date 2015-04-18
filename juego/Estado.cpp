#include "Estado.hpp"

#define VELOCIDAD_VERTICAL 75
#define VELOCIDAD_HORIZONTAL 60

using namespace std;
/***********************************************************************
 * 
 * 						ESTADO
 * 
 **********************************************************************/  
Estado::Estado(string ruta, SDL_Renderer* ren, float alto_personaje, float alto_escenario, float ancho_personaje, float ancho_escenario){
	this->quieto = new Quieto(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / alto_personaje), 0, 2*alto_personaje);
	this->saltarvertical = new SaltarVertical(ruta, ren, 3*VELOCIDAD_HORIZONTAL*(ancho_personaje / alto_personaje), VELOCIDAD_VERTICAL*(alto_personaje / alto_escenario), 2*alto_personaje);
	this->saltardiagonal = new SaltarDiagonal(ruta, ren, 3*VELOCIDAD_HORIZONTAL*(ancho_personaje / alto_personaje), VELOCIDAD_VERTICAL*(alto_personaje / alto_escenario), 2*alto_personaje);
	this->caminar = new Caminar(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / alto_personaje), 0, 2*alto_personaje);
}

Estado::~Estado(){
	delete this->quieto;
	delete this->caminar;
	delete this->saltarvertical;
	delete this->saltardiagonal;
}
