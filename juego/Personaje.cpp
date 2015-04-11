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

#define TEMPO 8000

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
Personaje::Personaje(CoordenadaLogica* coord, string nombre, SDL_Renderer* ren, float ancho_logico, float alto_logico){

	this->coordenada = coord;
	this->ancho = ancho_logico;
	this->alto = alto_logico;
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
void Personaje::cambiarAccionA(int nroAccion,string ruta, bool permiteInterrupcion){
	
	delete this->accionActual;
	this->accionActual = new Accion(nroAccion, ruta, permiteInterrupcion,this->renderer);
	this->lastTime = 0;	
	
}
/**Se encarga de determinar segun el tiempo transcurrido, qué imagen 
 * se debe mostrar por pantalla.
 * Recibe por parametro la nueva Accion que el loop del juego
 * quiere que el Personaje represente, 
 * y un puntero de tipo SDL_Renderer que indica el renderer usado.
 */ 
SDL_Texture* Personaje::definir_imagen(int nuevaAccion){
		
	int currentTime,tiempoTranscurrido;
	SDL_Texture* imagen_actual;
	string ruta = to_string(nuevaAccion);
	
	/*La accion que se esta representando no coincide con la nuevaAccion
	 * pasada por parametro
	 * El loop del juego esperaba que representara una nuevaAccion
	 * Si el juego recien se inicia, incluso entonces la nuevaAccion no coincide con el estado 
	 * inicial del Personaje
	 */
	 
	if (this->accionActual == NULL){
		this->accionActual = new Accion(0,"resources/jugador/SubZero/0",true,this->renderer);	//Accion default;
	}
	else if (this->accionActual->esDistintaA(nuevaAccion)){
		/*Se deben inicializar el vector de imagenes correspondientes a la secuencia
		 */
		string ruta2 = "resources/jugador/SubZero/"+ruta; 
		cambiarAccionA(nuevaAccion,ruta2,true);
		this->imagenActual = this->accionActual->getImagenActual();
		
	}
	/*Se desea continuar con el mismo movimiento
	 */
	else{
		currentTime = SDL_GetTicks();
		tiempoTranscurrido = currentTime - this->lastTime;
		/*Debo actualizar la imagen a mostrar
		 */ 
		if (tiempoTranscurrido > TEMPO){
				
				this->accionActual->cambiarModo();
				this->imagenActual = this->accionActual->getImagenActual();
				this->lastTime = this->lastTime + TEMPO;
		}
		else{
			this->imagenActual = this->accionActual->getImagenActual();
			/*Mantengo la imagen
			 * ergo: no hago nada
			 */ 
		}
	}
	
				
	return (this->imagenActual);
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
	return coord;
}
CoordenadaLogica* Personaje::obtenerCoordenadaIzqInf(){
}

CoordenadaLogica* Personaje::obtenerCoordenadaDerSup(){
	
}

CoordenadaLogica* Personaje::obtenerCoordenadaDerInf(){
	
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaIzqSup(){
	
	}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaIzqInf(){
	
	}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaDerSup(){
	
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaDerInf(){
	
}
