#include <stdio.h>
#include <iostream>
#include "Personaje.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>


using namespace std;

/***********************************************************************
 * 
 * 							CONSTANTES
 *
 **********************************************************************/  
#define QUIETO 0
#define CAMINAR_DERECHA 1
#define CAMINAR_IZQUIERDA 2
#define SALTAR 3
#define SALTODIAGONAL_DER 4
#define SALTODIAGONAL_IZQ 5

/***********************************************************************
 * 
 * 							CONSTRUCTOR
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
Personaje::Personaje(int posicion_x, int posicion_y, string nombre,SDL_Renderer* ren, Conf* parser){

	this->parser = parser;
	//~ this->parser->personaje_mirar_derecha = false;
	this->estado = new Estado((string)(this->parser->sprites_map["personaje1"]), ren);
	this->ladoDerecha = this->parser->personaje_mirar_derecha;
	this->nroAccionActual = 0;
	
	this->posicion_x = posicion_x;
	this->posicion_y = posicion_y;
	
	this->accionActual = this->estado->quieto;
	
	this->imagenActual = NULL;
	this->nombrePersonaje = nombre;
	this->renderer = ren;

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
void Personaje::cambiarAccionA(int nroAccion){
	
	this->accionActual->resetear();
	this->nroAccionActual = nroAccion;
	
	if (nroAccion == QUIETO){ 
			this->accionActual = this->estado->quieto;
	}if (nroAccion == CAMINAR_DERECHA){
			this->accionActual = this->estado->caminarder;
			if (!this->parser->personaje_mirar_derecha){
				//espejar e invertir las imagenes
				this->accionActual->setInvertirSecuencia();
			}
	}if (nroAccion == CAMINAR_IZQUIERDA){
			this->accionActual = this->estado->caminarder;
			if(this->parser->personaje_mirar_derecha){
				this->accionActual->setInvertirSecuencia();
			}
	}if (nroAccion == SALTAR){
			this->accionActual = this->estado->saltar;
	}if (nroAccion == SALTODIAGONAL_DER){
			this->accionActual = this->estado->saltardiagonal;
			if(!this->parser->personaje_mirar_derecha){
					//espejar e invertir las imagenes
				this->accionActual->setInvertirSecuencia();
			}
	}if (nroAccion == SALTODIAGONAL_IZQ){
			this->accionActual = this->estado->saltardiagonal;
			if(this->parser->personaje_mirar_derecha){
				this->accionActual->setInvertirSecuencia();
			}
	}
	//Se pasaron los casos de sprites especiales
	if(!this->parser->personaje_mirar_derecha){
		this->ladoDerecha = false;
	}
	
	

}
/**Se encarga de determinar segun el tiempo transcurrido, qué imagen 
 * se debe mostrar por pantalla.
 * Recibe por parametro la nueva Accion que el loop del juego
 * quiere que el Personaje represente, 
 * y un puntero de tipo SDL_Renderer que indica el renderer usado.
 */ 
 void Personaje::definir_imagen(int nuevaAccion){
	
	//~ puts("----------------------------------------------------------------------------------");	
	//~ cout<<"Accion actual: "<<this->nroAccionActual<<" Accion entratnte: "<<nuevaAccion<<endl;
	//~ cout<<"A la entrada estaba en el modo nro: "<<this->accionActual->getModoActual()<<endl;
	
	if (this->nroAccionActual != nuevaAccion){
		cambiarAccionA(nuevaAccion);
		this->imagenActual = this->accionActual->getImagenActual();
		return;
	}
	
	this->accionActual->execute();
	this->imagenActual = this->accionActual->getImagenActual();
	return;
	
}
Personaje::~Personaje(){
	
	delete this->estado;
	
}
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
