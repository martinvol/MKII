#include "Estado.hpp"


using namespace std;
/***********************************************************************
 * 
 * 						ESTADO
 * 
 **********************************************************************/  
Estado::Estado(string ruta, SDL_Renderer* ren){
	this->quieto = new Quieto(ruta, ren);
	this->saltar = new Saltar(ruta, ren);
	this->saltardiagonal = new SaltarDiagonal(ruta, ren);
	this->caminarder = new CaminarDerecha(ruta, ren);
}

Estado::~Estado(){
	delete this->quieto;
	delete this->caminarder;
	delete this->saltar;
	delete this->saltardiagonal;
}
