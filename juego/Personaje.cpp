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

#define TEMPO 30


/***********************************************************************
 * 
 * 							AUXILIAR
 *
 **********************************************************************/  

void imprimirMensaje (std::ostream &os, const std::string &msg, int num = NULL){
	os << msg << " : " << num << std::endl;
}


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
	this->estado = new Estado("resources/jugador/SubZero/", ren);
	this->posicion_x = posicion_x;
	this->posicion_y = posicion_y;
	this->accionActual = NULL;
	this->imagenActual = NULL;
	this->lastTime = 0;
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
void Personaje::cambiarAccionA(int nroAccion,string ruta){
	
	this->accionActual->resetear();
	//delete this->accionActual;
	if (nroAccion == 0){ 
			this->accionActual = this->estado->quieto;
	}if (nroAccion == 1){
			this->accionActual = this->estado->caminarder;
	}if (nroAccion == 2){
			this->accionActual = this->estado->caminarizq;
	}if (nroAccion == 3){
			this->accionActual = this->estado->saltar;
	}if (nroAccion == 4){
			this->accionActual = this->estado->saltardiagonal;
	}	

}
/**Se encarga de determinar segun el tiempo transcurrido, qué imagen 
 * se debe mostrar por pantalla.
 * Recibe por parametro la nueva Accion que el loop del juego
 * quiere que el Personaje represente, 
 * y un puntero de tipo SDL_Renderer que indica el renderer usado.
 */ 
SDL_Texture* Personaje::definir_imagen(int nuevaAccion){
		
	string ruta = this->parser->sprites_map["personaje1.png"];///"resources/jugador/SubZero/";
	
	if (this->accionActual == NULL){
		this->accionActual = estado->quieto;	//Accion default;
		this->imagenActual = this->accionActual->getImagenActual();
		return this->imagenActual;
	}else if (this->accionActual->esDistintaA(nuevaAccion)){
		if (this->accionActual->permite(nuevaAccion)){
			cambiarAccionA(nuevaAccion,ruta);
			this->imagenActual = this->accionActual->getImagenActual();
			return this->imagenActual;
		}
		if(this->accionActual->esUltimoModo()){
			cambiarAccionA(nuevaAccion,ruta);
			this->imagenActual = this->accionActual->getImagenActual();
			return this->imagenActual;
		}
	}
	
	this->accionActual->execute();
	this->imagenActual = this->accionActual->getImagenActual();
	return this->imagenActual;
	
}
Personaje::~Personaje(){
	
	delete this->accionActual;
	
}
/**
 * 
 */ 
void Personaje::Dibujarse(int x, int y){
    int ancho, alto;
	SDL_QueryTexture(this->imagenActual, NULL, NULL, &ancho, &alto);
	this->Dibujarse(x, y, alto, ancho);
}

/**
 * 
 * */
void Personaje::Dibujarse(int x, int y, int alto, int ancho){
	//Rectangulo destino
	SDL_Rect destino;
	destino.x = x;
	destino.y = y;
	destino.w = ancho;
	destino.h = alto;
	SDL_RenderCopy(this->renderer, this->imagenActual, NULL, &destino);
}


void Personaje::cambiar_posicion(int cant_pasos_x,int cant_pasos_y){
	
	
}
	
void Personaje::mirar_al_otro_lado(){
	
	
}
SDL_Texture* Personaje::DibujarSpriteNumero(int numeroDeSprite){
	return this->accionActual->getImagenNro(numeroDeSprite);
}
	
int Personaje::getSpriteActual(){
	return this->accionActual->getModoActual();
}
