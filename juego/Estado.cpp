#include "Estado.hpp"


using namespace std;
/***********************************************************************
 * 
 * 						ESTADO
 * 
 **********************************************************************/  
Estado::Estado(string ruta, SDL_Renderer* ren, float alto_personaje, float alto_escenario, float ancho_personaje, float ancho_escenario){
	this->quieto = new Quieto(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / alto_personaje), 0);
	this->saltarvertical = new SaltarVertical(ruta, ren, 3*VELOCIDAD_HORIZONTAL*(ancho_personaje / alto_personaje), VELOCIDAD_VERTICAL*(alto_personaje / alto_escenario));
	this->saltardiagonal = new SaltarDiagonal(ruta, ren, 3*VELOCIDAD_HORIZONTAL*(ancho_personaje / alto_personaje), VELOCIDAD_VERTICAL*(alto_personaje / alto_escenario));
	this->caminar = new Caminar(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / alto_personaje), 0);
}

Estado::~Estado(){
	delete this->quieto;
	delete this->caminar;
	delete this->saltarvertical;
	delete this->saltardiagonal;
}
