#include "Estado.hpp"

#define VELOCIDAD_VERTICAL 75*5
#define VELOCIDAD_HORIZONTAL 60*3

using namespace std;

/***********************************************************************
 * 
 * 						ESTADO
 * 
 **********************************************************************/  
Estado::Estado(string ruta, SDL_Renderer* ren, float alto_personaje, float alto_escenario, float ancho_personaje, float ancho_escenario, float ancho_ventana, int hue_init, int hue_fin, int hue_offset){
	this->quieto = new Quieto(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->saltarvertical = new SaltarVertical(ruta, ren, 0, VELOCIDAD_VERTICAL*(alto_personaje / ancho_escenario), 2*alto_personaje, hue_init, hue_fin, hue_offset);
	this->saltardiagonal = new SaltarDiagonal(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), VELOCIDAD_VERTICAL*(alto_personaje / ancho_escenario), 2*alto_personaje, hue_init, hue_fin, hue_offset);
	this->caminar = new Caminar(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->agacharse = new Agacharse(ruta, ren, 0, VELOCIDAD_VERTICAL*(alto_personaje / ancho_escenario), 2*alto_personaje, hue_init, hue_fin, hue_offset);
	this->piniaAlta = new PiniaAlta(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->piniaBaja = new PiniaBaja(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->patadaAlta = new PatadaAlta(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->patadaBaja = new PatadaBaja(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->patadaBajaAgachado = new PatadaBajaAgachado(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->patadaAltaAgachado = new PatadaAltaAgachado(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->girar = new Girar(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->traba = new Traba(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->cubrirAlto = new CubrirAlto(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->cubrirBajo = new CubrirBajo(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->ruta_arrojable = ruta + "arma.png";
	
	this->piniaAire = new PiniaAire(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), VELOCIDAD_VERTICAL*(alto_personaje / ancho_escenario), 2*alto_personaje, hue_init, hue_fin, hue_offset);
	this->patadaDiag = new PatadaSaltoDiagonal(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), VELOCIDAD_VERTICAL*(alto_personaje / ancho_escenario), 2*alto_personaje, hue_init, hue_fin, hue_offset);
	this->patadaVert = new PatadaSaltoVertical(ruta, ren, 0, VELOCIDAD_VERTICAL*(alto_personaje / ancho_escenario), 2*alto_personaje, hue_init, hue_fin, hue_offset);
	this->piniaAireVertical = new PiniaAire(ruta, ren, 0, VELOCIDAD_VERTICAL*(alto_personaje / ancho_escenario), 2*alto_personaje, hue_init, hue_fin, hue_offset);
}

Estado::~Estado(){
	delete this->quieto;
	delete this->caminar;
	delete this->saltarvertical;
	delete this->saltardiagonal;
	delete this->agacharse;
	delete this->piniaAlta;
	delete this->piniaBaja;
	delete this->patadaAlta;
	delete this->patadaBaja;
	delete this->patadaBajaAgachado;
	delete this->patadaAltaAgachado;
	delete this->girar;
	delete this->traba;
	delete this->cubrirAlto;
	delete this->cubrirBajo;
	delete this->piniaAire;
	delete this->patadaDiag;
	delete this->patadaVert;
	delete this->piniaAireVertical;
}
