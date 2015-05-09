#include "Estado.hpp"

#define VELOCIDAD_VERTICAL 75*5
#define VELOCIDAD_HORIZONTAL 60*3

using namespace std;
/***********************************************************************
 * 
 * 						ESTADO
 * 
 **********************************************************************/  
Estado::Estado(string ruta, SDL_Renderer* ren, float alto_personaje, float alto_escenario, float ancho_personaje, float ancho_escenario, float ancho_ventana){
	this->quieto = new Quieto(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0);
	this->saltarvertical = new SaltarVertical(ruta, ren, 0, VELOCIDAD_VERTICAL*(alto_personaje / ancho_escenario), 2*alto_personaje);
	this->saltardiagonal = new SaltarDiagonal(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), VELOCIDAD_VERTICAL*(alto_personaje / ancho_escenario), 2*alto_personaje);
	this->caminar = new Caminar(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0);
	this->agacharse = new Agacharse(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0);
	this->piniaAlta = new PiniaAlta(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0);
	this->piniaBaja = new PiniaBaja(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0);
}
Estado::~Estado(){
	delete this->quieto;
	delete this->caminar;
	delete this->saltarvertical;
	delete this->saltardiagonal;
	delete this->agacharse;
	delete this->piniaAlta;
	delete this->piniaBaja;
}
