#include <stdio.h>
#include <iostream>
#include "Personaje.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

using namespace std;


/***********************************************************************
 * 
 * 							AUXILIAR
 *
 **********************************************************************/  

//~ void imprimirMensaje (std::ostream &os, const std::string &msg, int num = NULL){
	//~ os << msg << " : " << num << std::endl;
//~ }


/***********************************************************************
 * 
 * 							CONSTRUCTOR
 * 						   Y DESTRUCTOR
 *
 **********************************************************************/  

/**Recibe por parametro la posicion inicial del personaje, representado
 * por una coordenada.
 * El nombre del personaje, que coincide con el nombre de la carpeta donde
 * se guardan las imagenes de las acciones.
 * y un puntero de tipo SDL_Renderer que indica el renderer usado.
 * */
/* HEAD
Personaje::Personaje(CoordenadaLogica* coordenada, string nombre,SDL_Renderer* ren, float alto, float ancho, Estado* estado, Conf* parser){

	this->parser= parser;

	this->ancho = ancho;
	this->alto = alto;
	this->estado = estado;
	this->ladoDerecha = this->parser->personaje_mirar_derecha;
	this->nroAccionActual = 0;

	this->y_inicial = coordenada->y;
	this->coordenada = coordenada;
	this->siguiente = NULL;
	
	this->accionActual = this->estado->quieto;
	
	this->imagenActual = NULL;
	this->nombrePersonaje = nombre;
	this->renderer = ren;

}


Personaje::~Personaje(){
	
	delete this->estado;
	
}
*/

Personaje::Personaje(CoordenadaLogica* coord, string nombre,SDL_Renderer* ren, float alto, float ancho, Estado* estado, bool derecha){
//~ Personaje::Personaje(CoordenadaLogica* coord, string nombre,SDL_Renderer* ren, float alto, float ancho, Estado* estado){

	this->alto = alto;
	this->ancho = ancho;
	
	this->y_inicial = coord->y;
	this->coordenada = coord;
	this->siguiente = NULL;
	
	this->mirarDerecha = derecha;
	
	this->estado = estado;
	this->nroAccionActual = QUIETO;
	this->accionActual = this->estado->quieto;
	this->imagenActual = this->accionActual->getImagenActual();
	
	this->nombrePersonaje = nombre;
	this->renderer = ren;
	
}

Personaje::~Personaje(){
	delete this->coordenada;
	delete this->estado;	// Esto elimina la acción y sus imágenes.
}

/***********************************************************************
 * 
 * 					FUNCIONES PARA EL DIRECTOR
 *
 **********************************************************************/  

void Personaje::mirarParaDerecha(){
	mirarDerecha = true;
}

void Personaje::mirarParaIzquierda(){
	mirarDerecha = false;
}

void Personaje::activarAccion(accion_posible accion){
//~ puts("----------------------------------------------------------------------------------");	
//~ cout<<"Accion actual: "<<this->nroAccionActual<<" Accion entratnte: "<<nuevaAccion<<endl;
//~ cout<<"A la entrada estaba en el modo nro: "<<this->accionActual->getModoActual()<<endl;
//	if(this->accionActual == NULL) puts ("por que");
	if (this->nroAccionActual != accion && (this->accionActual->permiteAccion(accion))){
		cambiarAccionA(accion);
	} else {
		if (siguiente != NULL){ delete siguiente; }
		siguiente = this->accionActual->execute(this->coordenada);
		switch (nroAccionActual){
			case SALTAR:
			case SALTARDIAGONAL_DER:
			case SALTARDIAGONAL_IZQ:
				if (siguiente->y < y_inicial){
					cambiarAccionA(QUIETO);
					CoordenadaLogica* coord = new CoordenadaLogica(siguiente->x, y_inicial);
					delete siguiente;
					siguiente = coord;
				}
			default:
				break;
		}
	}
//~ if (nroAccionActual == AGACHARSE)
//~ cout<<"quiero agacharme"<<endl;
	this->imagenActual = this->accionActual->getImagenActual();
}

CoordenadaLogica* Personaje::obtenerCoordenadaIzqSup(){
	CoordenadaLogica* coord = new CoordenadaLogica(coordenada);
	coord->desplazarY(alto);
	return coord;
}

CoordenadaLogica* Personaje::obtenerCoordenadaIzqInf(){
	CoordenadaLogica* coord = new CoordenadaLogica(coordenada);
	return coord;
}

CoordenadaLogica* Personaje::obtenerCoordenadaDerSup(){
	CoordenadaLogica* coord = new CoordenadaLogica(coordenada);
	coord->desplazarY(alto);
	coord->desplazarX(ancho);
	return coord;
}

CoordenadaLogica* Personaje::obtenerCoordenadaDerInf(){
	CoordenadaLogica* coord = new CoordenadaLogica(coordenada);
	coord->desplazarX(ancho);
	return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaIzqSup(){
	if (siguiente == NULL) return NULL;
	CoordenadaLogica* coord = new CoordenadaLogica(siguiente);
	coord->desplazarY(alto);
	return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaIzqInf(){
	if (siguiente == NULL) return NULL;
	CoordenadaLogica* coord = new CoordenadaLogica(siguiente);
	return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaDerSup(){
	if (siguiente == NULL) return NULL;
	CoordenadaLogica* coord = new CoordenadaLogica(siguiente);
	coord->desplazarY(alto);
	coord->desplazarX(ancho);
	return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaDerInf(){
	if (siguiente == NULL) return NULL;
	CoordenadaLogica* coord = new CoordenadaLogica(siguiente);
	coord->desplazarX(ancho);
	return coord;
}

void Personaje::moverseAIzqSup(CoordenadaLogica* coord){
	if (!this->coordenada) delete coordenada;
	coordenada = coord;
	coordenada->desplazarY(-alto);
}

void Personaje::moverseADerSup(CoordenadaLogica* coord){
	if (!this->coordenada) delete coordenada;
	coordenada = coord;
	coordenada->desplazarY(-alto);
	coordenada->desplazarX(-ancho);
}

/***********************************************************************
 * 
 * 							DEMAS
 *
 **********************************************************************/  
/* Cuando se espera que el personaje represente una nueva accion, 
 * se destruye la Accion guardada
 * y se inicializa una nueva.
 * El tiempo se setea nuevamente en 0.
 * Recibe por parametro el numero que representa la nueva accion
 * un booleano que indica si la accion puede ser interrumpida.
 * y un puntero de tipo SDL_Renderer que indica el renderer.
 * */
void Personaje::cambiarAccionA(accion_posible nroAccion){
	
	this->accionActual->resetear();
	this->nroAccionActual = nroAccion;
	
	switch (nroAccionActual)
	{ 
		case QUIETO:
			this->accionActual = this->estado->quieto;
			break;
		case CAMINAR_DERECHA:
			this->accionActual = this->estado->caminar;
			if (!this->mirarDerecha){
				this->accionActual->setInvertirSecuencia();
			}
			break;
		case CAMINAR_IZQUIERDA:
			this->accionActual = this->estado->caminar;
			if(this->mirarDerecha){
				this->accionActual->setInvertirSecuencia();
			}
			break;
		case SALTAR:
			this->accionActual = this->estado->saltarvertical;
			break;
		case AGACHARSE:
			this->accionActual = this->estado->agacharse;			
			break;
		case SALTARDIAGONAL_DER:
			this->accionActual = this->estado->saltardiagonal;
			if(!this->mirarDerecha){
				this->accionActual->setInvertirSecuencia();
			}
			break;		
		default: // case SALTARDIAGONAL_IZQ:
			this->accionActual = this->estado->saltardiagonal;
			if(this->mirarDerecha){
				this->accionActual->setInvertirSecuencia();
			}
			break;
	}
}


void Personaje::Dibujarse(ConversorDeCoordenadas* conv){
	CoordenadaLogica* coord1 = this->obtenerCoordenadaIzqInf();
	CoordenadaFisica* coord1_fis = conv->aFisica(coord1);
	
	CoordenadaLogica* coord2 = this->obtenerCoordenadaDerSup();
	CoordenadaFisica* coord2_fis = conv->aFisica(coord2);
	
	int ancho_fisico = abs(coord1_fis->x_fisico - coord2_fis->x_fisico);		// Función de std
	int alto_fisico = abs(coord1_fis->y_fisico - coord2_fis->y_fisico);
	
	//Rectangulo destino
	SDL_Rect destino;
	destino.x = coord1_fis->x_fisico;
	destino.y = coord2_fis->y_fisico;
	destino.w = ancho_fisico;
	destino.h = alto_fisico;
	
	// Espeja si debe mirar para la izquierda.
	if (!this->mirarDerecha){
		SDL_RenderCopyEx(this->renderer, this->imagenActual, NULL, &destino,0,NULL,SDL_FLIP_HORIZONTAL);
	} else {
		SDL_RenderCopyEx(this->renderer, this->imagenActual, NULL, &destino,0,NULL,SDL_FLIP_NONE);
	}
	
	delete coord1;
	delete coord1_fis;
	delete coord2;
	delete coord2_fis;
	
}


//~ COSAS QUE NO IRÍAN
//~ void Personaje::Dibujarse(int x, int y){
    //~ int ancho, alto;
	//~ SDL_QueryTexture(this->imagenActual, NULL, NULL, &ancho, &alto);
	//~ this->Dibujarse(x, y, float(alto), float(ancho));
//~ }

//~ void Personaje::Dibujarse(int x, int y, float alto, float ancho){
	//~ //Rectangulo destino
	//~ SDL_Rect destino;
	//~ destino.x = x;
	//~ destino.y = y;
	//~ destino.w = ancho;
	//~ destino.h = alto;
	//~ if (!this->ladoDerecha){
		//~ SDL_RenderCopyEx(this->renderer, this->imagenActual, NULL, &destino,0,NULL,SDL_FLIP_HORIZONTAL);
	//~ }
	//~ else{
		//~ SDL_RenderCopyEx(this->renderer, this->imagenActual, NULL, &destino,0,NULL,SDL_FLIP_NONE);
	//~ }
//~ }

/***********************************************************************
 * 
 * 							AUXILIAR
 *
 **********************************************************************/  

//~ SDL_Texture* Personaje::DibujarSpriteNumero(int numeroDeSprite){
	//~ return this->accionActual->getImagenNro(numeroDeSprite);
//~ }
	//~ 
//~ int Personaje::getSpriteActual(){
	//~ return this->accionActual->getModoActual();
//~ }
