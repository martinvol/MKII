#include <stdio.h>
#include <iostream>
#include "Personaje.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>



using namespace std;

/***********************************************************************
 * 
 * 						CONSTRUCTOR Y DESTRUCTOR
 *
 **********************************************************************/  

/**Recibe por parametro la posicion inicial del personaje, representado
 * por dos enteros: 
 * uno) marca la posicion en el eje x,
 * dos) marcla la posicion en el eje y.
 * El nombre del personaje, que coincide con el nombre de la carpeta donde
 * se guardan las imagenes de las acciones.
 * y un puntero de tipo SDL_Renderer que indica el renderer usado.
 * */
Personaje::Personaje(CoordenadaLogica* coordenada, string nombre,SDL_Renderer* ren, float alto, float ancho, Estado* estado, Conf* parser){

	this->parser= parser;

	this->ancho = ancho;
	this->alto = alto;
	//~ this->parser->personaje_mirar_derecha = false;
	this->estado = estado;
	this->ladoDerecha = this->parser->personaje_mirar_derecha;
	this->nroAccionActual = 0;
	
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

/***********************************************************************
 * 
 * 							DEMAS
 *
 **********************************************************************/  
/**Cuando se espera que el personaje represente una nueva accion, 
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
	
	switch (nroAccion)
	{ 
		case QUIETO:
			this->accionActual = this->estado->quieto;
			break;
		case CAMINAR_DERECHA:
			this->accionActual = this->estado->caminar;
			if (!this->parser->personaje_mirar_derecha){
				//espejar e invertir las imagenes
				this->accionActual->setInvertirSecuencia();
			}
			break;
		case CAMINAR_IZQUIERDA:
			this->accionActual = this->estado->caminar;
			if(this->parser->personaje_mirar_derecha){
				this->accionActual->setInvertirSecuencia();
			}
			break;
		case SALTAR:
			this->accionActual = this->estado->saltarvertical;
			break;
		case SALTARDIAGONAL_DER:
			this->accionActual = this->estado->saltardiagonal;
			if(!this->parser->personaje_mirar_derecha){
					//espejar e invertir las imagenes
				this->accionActual->setInvertirSecuencia();
			}
			break;
		default: // SALTARDIAGONAL_IZQ:
			this->accionActual = this->estado->saltardiagonal;
			if(this->parser->personaje_mirar_derecha){
				this->accionActual->setInvertirSecuencia();
			}
			break;
	}
	//Se pasaron los casos de sprites especiales
	if(!this->parser->personaje_mirar_derecha){
		this->ladoDerecha = false;
	}
	
	

}
//~ /**Se encarga de determinar segun el tiempo transcurrido, qué imagen 
 //~ * se debe mostrar por pantalla.
 //~ * Recibe por parametro la nueva Accion que el loop del juego
 //~ * quiere que el Personaje represente, 
 //~ * y un puntero de tipo SDL_Renderer que indica el renderer usado.
 //~ */ 
 //~ void Personaje::definir_imagen(accion_posible nuevaAccion){
	//~ 
	//~ puts("----------------------------------------------------------------------------------");	
	//~ cout<<"Accion actual: "<<this->nroAccionActual<<" Accion entratnte: "<<nuevaAccion<<endl;
	//~ cout<<"A la entrada estaba en el modo nro: "<<this->accionActual->getModoActual()<<endl;
	//~ 
	//~ if (this->nroAccionActual != nuevaAccion){
		//~ cambiarAccionA(nuevaAccion);
		//~ this->imagenActual = this->accionActual->getImagenActual();
		//~ return;
	//~ }
	//~ 
	//~ this->accionActual->execute();
	//~ this->imagenActual = this->accionActual->getImagenActual();
	//~ return;	
//~ }

/**
 * 
 */ 
void Personaje::Dibujarse(int x, int y){
    int ancho, alto;
	SDL_QueryTexture(this->imagenActual, NULL, NULL, &ancho, &alto);
	this->Dibujarse(x, y, float(alto), float(ancho));
}

/**
 * 
 * */
void Personaje::Dibujarse(int x, int y, float alto, float ancho){
	//Rectangulo destino
	SDL_Rect destino;
	destino.x = x;
	destino.y = y;
	destino.w = ancho;
	destino.h = alto;
	if (!this->ladoDerecha){
		SDL_RenderCopyEx(this->renderer, this->imagenActual, NULL, &destino,0,NULL,SDL_FLIP_HORIZONTAL);
	}
	else{
		SDL_RenderCopyEx(this->renderer, this->imagenActual, NULL, &destino,0,NULL,SDL_FLIP_NONE);
	}
}


/***********************************************************************
 * 
 * 							AUXILIAR
 *
 **********************************************************************/  

//~ void imprimirMensaje (std::ostream &os, const std::string &msg, int num = NULL){
	//~ os << msg << " : " << num << std::endl;
//~ }


//~ 
//~ void Personaje::cambiar_posicion(int cant_pasos_x,int cant_pasos_y){
	//~ 
	//~ 
//~ }
	//~ 
//~ void Personaje::mirar_al_otro_lado(){
	//~ 
	//~ 
//~ }
//~ SDL_Texture* Personaje::DibujarSpriteNumero(int numeroDeSprite){
	//~ return this->accionActual->getImagenNro(numeroDeSprite);
//~ }
	//~ 
//~ int Personaje::getSpriteActual(){
	//~ return this->accionActual->getModoActual();
//~ }


/***********************************************************************
 * 
 * 							PARA EL DIRECTOR
 *
 **********************************************************************/ 
//~ 
//~ void Personaje::mirarParaDerecha(){
//~ 
//~ }
//~ 
//~ void Personaje::mirarParaIzquierda(){
//~ 
//~ }
//~ 

/* Reemplaza al DEFINIR_IMAGEN, con más cosas */
void Personaje::activarAccion(accion_posible accion){
	definir_imagen(accion);
}

void Personaje::definir_imagen(accion_posible accion){
	if (this->nroAccionActual != accion && (this->accionActual->permiteAccion(accion))){
		cambiarAccionA(accion);
	} else {
		if (siguiente != NULL) delete siguiente;
		siguiente = this->accionActual->execute(this->coordenada);
		switch (nroAccionActual){
			case SALTAR:
			case SALTARDIAGONAL_DER:
			case SALTARDIAGONAL_IZQ:
				if (siguiente == NULL){
					cambiarAccionA(QUIETO);
					siguiente = coordenada;
				}
			default:
				break;
		}
	}
	this->imagenActual = this->accionActual->getImagenActual();
	return;
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
	CoordenadaLogica* coord = new CoordenadaLogica(siguiente);
	coord->desplazarY(alto);
	return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaIzqInf(){
	CoordenadaLogica* coord = new CoordenadaLogica(siguiente);
	return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaDerSup(){
	CoordenadaLogica* coord = new CoordenadaLogica(siguiente);
	coord->desplazarY(alto);
	coord->desplazarX(ancho);
	return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaDerInf(){
	CoordenadaLogica* coord = new CoordenadaLogica(siguiente);
	coord->desplazarX(ancho);
	return coord;
}

void Personaje::moverseAIzqSup(CoordenadaLogica* coord){
	delete coordenada;
	coordenada = coord;
	coord->desplazarY(-alto);
}

void Personaje::moverseADerSup(CoordenadaLogica* coord){
	delete coordenada;
	coordenada = coord;
	coord->desplazarY(-alto);
	coord->desplazarX(-ancho);
}
