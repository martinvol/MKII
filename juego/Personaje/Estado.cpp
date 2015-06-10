#include "Estado.hpp"

#define VELOCIDAD_VERTICAL 75*4
#define VELOCIDAD_HORIZONTAL 50

using namespace std;

/***********************************************************************
 * 
 * 						ESTADO
 * 
 **********************************************************************/  
Estado::Estado(string ruta, SDL_Renderer* ren, float alto_personaje, float alto_escenario, float ancho_personaje, float ancho_escenario, float ancho_ventana, int hue_init, int hue_fin, int hue_offset){
	this->quieto = new Quieto(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->saltarvertical = new SaltarVertical(ruta, ren, 0, (2./3.)*VELOCIDAD_VERTICAL*(alto_personaje / ancho_escenario), alto_personaje, hue_init, hue_fin, hue_offset);
	this->saltardiagonal = new SaltarDiagonal(ruta, ren, 3*VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), VELOCIDAD_VERTICAL*(alto_personaje / ancho_escenario), 2*alto_personaje, hue_init, hue_fin, hue_offset);
	this->caminar = new Caminar(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->agacharse = new Agacharse(ruta, ren, 0, VELOCIDAD_VERTICAL*(alto_personaje / ancho_escenario), 2*alto_personaje, hue_init, hue_fin, hue_offset);
	this->piniaAlta = new PiniaAlta(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->piniaBaja = new PiniaBaja(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->piniaAgachado = new PiniaAgachado(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->patadaAlta = new PatadaAlta(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->patadaBaja = new PatadaBaja(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->patadaBajaAgachado = new PatadaBajaAgachado(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->patadaAltaAgachado = new PatadaAltaAgachado(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->girar = new Girar(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->traba = new Traba(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->cubrirAlto = new CubrirAlto(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->cubrirBajo = new CubrirBajo(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->ruta_arrojable = ruta + "arma.png";
	this->levantarse = new Levantarse(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->levantarseDelGancho = new LevantarseDelGancho(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);

	this->roundKick = new RoundKick(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->gancho = new Gancho(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	
	this->piniaAire = new PiniaAire(ruta, ren, 3*VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), VELOCIDAD_VERTICAL*(alto_personaje / ancho_escenario), 2*alto_personaje, hue_init, hue_fin, hue_offset);
	this->patadaDiag = new PatadaSaltoDiagonal(ruta, ren, 3*VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), VELOCIDAD_VERTICAL*(alto_personaje / ancho_escenario), 2*alto_personaje, hue_init, hue_fin, hue_offset);
	this->patadaVert = new PatadaSaltoVertical(ruta, ren, 0, (2./3.)*VELOCIDAD_VERTICAL*(alto_personaje / ancho_escenario), alto_personaje, hue_init, hue_fin, hue_offset);
	this->piniaAireVertical = new PiniaAire(ruta, ren, 0, (2./3.)*VELOCIDAD_VERTICAL*(alto_personaje / ancho_escenario), alto_personaje, hue_init, hue_fin, hue_offset);
	
	this->recibirgolpealto = new RecibirGolpeAlto(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->recibirgolpebajo = new RecibirGolpeBajo(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->recibirTraba = new RecibirTraba(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->recibirGolpeAgachado = new RecibirGolpeAgachado(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->siendoarrojado = new SiendoArrojado(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0.5*VELOCIDAD_VERTICAL*(alto_personaje / ancho_escenario), 2*alto_personaje, hue_init, hue_fin, hue_offset);
	this->arrojarproyectil = new ArrojarProyectil(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	
	this->ganar = new Ganar(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->dizzy = new Dizzy(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->desaparecer = new Desaparecer(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->morir = new Morir(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset);
	this->desenmascararse = new Perform(ruta, ren, VELOCIDAD_HORIZONTAL*(ancho_personaje / ancho_ventana), 0, 0, hue_init, hue_fin, hue_offset, 98);
	
	this->piniaBaja->setDanio(50);
	this->patadaDiag->setDanio(50);
	this->piniaAlta->setDanio(70);
	this->patadaBaja->setDanio(60);
	this->patadaVert->setDanio(60);
	this->patadaAlta->setDanio(80);
	
	this->patadaBajaAgachado->setDanio(30);
	this->piniaAgachado->setDanio(30);
	this->patadaAltaAgachado->setDanio(40);
	this->piniaAireVertical->setDanio(40);
	this->piniaAire->setDanio(40);
	this->traba->setDanio(40);

	
	//~ this->piniaAire->setDanio();
	//~ this->piniaAireVertical->setDanio();	
	//~ this->patadaDiag->setDanio();
	//~ this->patadaVert->setDanio();	
	
	this->gancho->setDanio(90);
	this->roundKick->setDanio(70);
}

Estado::~Estado(){
	delete this->quieto;
	delete this->caminar;
	delete this->saltarvertical;
	delete this->saltardiagonal;
	delete this->agacharse;
	delete this->piniaAlta;
	delete this->piniaBaja;
	delete this->piniaAgachado;
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
	delete this->gancho;
	delete this->roundKick;
	delete this->recibirgolpealto;
	delete this->recibirgolpebajo;
	delete this->recibirTraba;
	delete this->siendoarrojado;
	delete this->levantarse;
	delete this->levantarseDelGancho;
	delete this->arrojarproyectil;
	delete this->recibirGolpeAgachado;
	delete this->ganar;
	delete this->morir;
	delete this->desaparecer;
	delete this->dizzy;
}
