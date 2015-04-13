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

Personaje::Personaje(CoordenadaLogica* coord, string nombre,SDL_Renderer* ren, Parser* parser){

	this->parser = parser;
	this->estado = new Estado((string)(this->parser->sprites_map["personaje1"]), ren, this->parser);
	this->coordenada = coord;
	this->accionActual = this->estado->quieto;
	this->imagenActual = NULL;
	this->lastTime = 0;
	this->nombrePersonaje = nombre;
	this->renderer = ren;

}

/***********************************************************************
 * 
 * 					FUNCIONES PARA EL DIRECTOR
 *
 **********************************************************************/  

/* FALTAN PARA INTERACCION CON EL DIRECTOR */
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
 void Personaje::definir_imagen(float tmp, int nuevaAccion){
	
	puts("----------------------------------------------------------------------------------");	
	cout<<"Accion actual: "<<this->accionActual->accionNro<<" Accion entratnte: "<<nuevaAccion<<endl;
	cout<<"La accion actual permite cambio?: "<< this->accionActual->permite(nuevaAccion)<<endl;
	cout<<"A la entrada estaba en el modo nro: "<<this->accionActual->getModoActual()<<endl;
	
	if (this->accionActual == NULL){
		this->accionActual = estado->quieto;	//Accion default;
		this->imagenActual = this->accionActual->getImagenActual();
		return;// this->imagenActual;
	}else if (this->accionActual->esDistintaA(nuevaAccion)){
		
		if (this->accionActual->permite(nuevaAccion)){
			puts("Cambie de accion porque lo permitia");
			cambiarAccionA(nuevaAccion);
			//this->accionActual->execute();
			this->imagenActual = this->accionActual->getImagenActual();
			return;// this->imagenActual;
		}
		if(this->accionActual->esUltimoModo()){
			puts("Llego al ultimo estado");
			cambiarAccionA(nuevaAccion);
			//this->accionActual->execute();
			this->imagenActual = this->accionActual->getImagenActual();
			return;// this->imagenActual;
		}
		//~ cambiarAccionA(nuevaAccion);
		//~ this->accionActual->execute();
		//~ this->imagenActual = this->accionActual->getImagenActual();
		//~ return this->imagenActual;
	}
	
	this->accionActual->execute(tmp);
	cout<<"A la salida muestro la imagen del  modo nro: "<<this->accionActual->getModoActual()<<endl;
	this->imagenActual = this->accionActual->getImagenActual();
	return;// this->imagenActual;
	
}
Personaje::~Personaje(){
	
	delete this->estado;
	//~ delete this->accionActual;
	
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
	SDL_RenderCopy(this->renderer, this->imagenActual, NULL, &destino);
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
