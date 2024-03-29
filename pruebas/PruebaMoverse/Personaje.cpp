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
Personaje::Personaje(int posicion_x, int posicion_y, string nombre,SDL_Renderer* ren){

	this->posicion_x = posicion_x;
	this->posicion_y = posicion_y;
	this->accionActual = NULL;
	this->imagenActual = NULL;
	this->lastTime = 0;
	this->nombrePersonaje = nombre;

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
void Personaje::cambiarAccionA(int nroAccion,string ruta, bool permiteInterrupcion,SDL_Renderer* ren){
	
	this->accionActual->destruirAccion();
	this->accionActual = new Accion(nroAccion, ruta, permiteInterrupcion,ren);
	this->lastTime = 0;	
	
}
/**Se encarga de determinar segun el tiempo transcurrido, qué imagen 
 * se debe mostrar por pantalla.
 * Recibe por parametro la nueva Accion que el loop del juego
 * quiere que el Personaje represente, 
 * y un puntero de tipo SDL_Renderer que indica el renderer usado.
 */ 
SDL_Texture* Personaje::definir_imagen(int nuevaAccion,SDL_Renderer* ren){
		
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
		this->accionActual = new Accion(0,"0",true,ren);	//Accion default;
	
	}
	else if (this->accionActual->esDistintaA(nuevaAccion)){
		/*Se deben inicializar el vector de imagenes correspondientes a la secuencia
		 */
		cambiarAccionA(nuevaAccion,ruta,true,ren);
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

void Personaje::destruirPersonaje(){
	
	this->accionActual->destruirAccion();
	
}

void Personaje::cambiar_posicion(int cant_pasos_x,int cant_pasos_y){
	
	
}
	
void Personaje::mirar_al_otro_lado(){
	
	
}
