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
 * por dos enteros: 
 * uno) marca la posicion en el eje x,
 * dos) marcla la posicion en el eje y.
 * El nombre del personaje, que coincide con el nombre de la carpeta donde
 * se guardan las imagenes de las acciones.
 * y un puntero de tipo SDL_Renderer que indica el renderer usado.
 * */

Personaje::Personaje(CoordenadaLogica* coord, string nombre,SDL_Renderer* ren, string ruta){

	this->estado = new Estado(ruta, ren);
	this->coordenada = coord;
	this->ladoDerecha = true; //this->parser->personaje_mirar_derecha;
	this->nroAccionActual = 0;
	this->accionActual = this->estado->quieto;
	this->imagenActual = NULL;
	this->lastTime = 0;
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

}

void Personaje::mirarParaIzquierda(){

}

void Personaje::parar(){

}

void Personaje::caminarDerecha(){

}

void Personaje::caminarIzquierda(){

}

void Personaje::saltar(){

}

void Personaje::saltarDerecha(){

}

void Personaje::saltarIzquierda(){

}

CoordenadaLogica* Personaje::obtenerCoordenadaIzqSup(){
	CoordenadaLogica* coord = new CoordenadaLogica(0.0,0.0);
	coord->sumar(coordenada);
	coord->desplazarY(alto);
	return coord;
}

CoordenadaLogica* Personaje::obtenerCoordenadaIzqInf(){
	CoordenadaLogica* coord = new CoordenadaLogica(0.0,0.0);
	coord->sumar(coordenada);
	return coord;
}

CoordenadaLogica* Personaje::obtenerCoordenadaDerSup(){
	CoordenadaLogica* coord = new CoordenadaLogica(0.0,0.0);
	coord->sumar(coordenada);
	coord->desplazarY(alto);
	coord->desplazarX(ancho);
	return coord;
}

CoordenadaLogica* Personaje::obtenerCoordenadaDerInf(){
	CoordenadaLogica* coord = new CoordenadaLogica(0.0,0.0);
	coord->sumar(coordenada);
	coord->desplazarX(ancho);
	return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaIzqSup(){
	//~ CoordenadaLogica* coord = obtengo la proxima preguntandole a la accion o de alguna forma.
	//~ Me devuelve una coordenada nueva que debo liberar.
	//~ coord->desplazarY(alto);
	//~ return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaIzqInf(){
	//~ CoordenadaLogica* coord = obtengo la proxima preguntandole a la accion o de alguna forma.
	//~ Me devuelve una coordenada nueva que debo liberar.
	//~ return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaDerSup(){
	//~ CoordenadaLogica* coord = obtengo la proxima preguntandole a la accion o de alguna forma.
	//~ Me devuelve una coordenada nueva que debo liberar.
	//~ coord->desplazarY(alto);
	//~ coord->desplazarX(ancho);
	//~ return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaDerInf(){
	//~ CoordenadaLogica* coord = obtengo la proxima preguntandole a la accion o de alguna forma.
	//~ Me devuelve una coordenada nueva que debo liberar.
	//~ coord->desplazarX(ancho);
	//~ return coord;
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
	if (nroAccion == 0){ 
			this->accionActual = this->estado->quieto;
	}if (nroAccion == 1){
			this->accionActual = this->estado->caminarder;
			if (!this->parser->personaje_mirar_derecha){
				//espejar e invertir las imagenes
				this->accionActual->setInvertirSecuencia();
				this->ladoDerecha = false;
				
				return;
			}
	}if (nroAccion == 2){
			this->accionActual = this->estado->caminarder;
			if(this->parser->personaje_mirar_derecha){
				this->ladoDerecha = true;
				this->accionActual->setInvertirSecuencia();
				return;
			}
	}if (nroAccion == 3){
			this->accionActual = this->estado->saltar;
	}if (nroAccion == 4){
			this->accionActual = this->estado->saltardiagonal;
	}	
	this->ladoDerecha = true;
	
	

}
/**Se encarga de determinar segun el tiempo transcurrido, qué imagen 
 * se debe mostrar por pantalla.
 * Recibe por parametro la nueva Accion que el loop del juego
 * quiere que el Personaje represente, 
 * y un puntero de tipo SDL_Renderer que indica el renderer usado.
 */ 
 void Personaje::definir_imagen(float tmp, int nuevaAccion){
	
	//~ puts("----------------------------------------------------------------------------------");	
	//~ cout<<"Accion actual: "<<this->accionActual->accionNro<<" Accion entratnte: "<<nuevaAccion<<endl;
	//~ cout<<"La accion actual permite cambio?: "<< this->accionActual->permite(nuevaAccion)<<endl;
	//~ cout<<"A la entrada estaba en el modo nro: "<<this->accionActual->getModoActual()<<endl;
	//~ if(!this->ladoDerecha && nuevaAccion==1){nuevaAccion=2;}
	//~ else if(!this->ladoDerecha && nuevaAccion==2){nuevaAccion=1;}
	if(nuevaAccion == 5){nuevaAccion=4;}
	
	if (this->nroAccionActual != nuevaAccion){
		//~ puts("entra Aca");
		cambiarAccionA(nuevaAccion);
		this->imagenActual = this->accionActual->getImagenActual();
		return;// this->imagenActual;
	}
	
	this->accionActual->execute(tmp);
	//~ cout<<"A la salida muestro la imagen del  modo nro: "<<this->accionActual->getModoActual()<<endl;
	this->imagenActual = this->accionActual->getImagenActual();
	return;// this->imagenActual;
	
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
void Personaje::cambiar_posicion(int cant_pasos_x,int cant_pasos_y){
	
	
}
	
void Personaje::mirar_al_otro_lado(){
	
	
}

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
