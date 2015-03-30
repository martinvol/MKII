#include <stdio.h>
#include <iostream>
#include "Personaje.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


using namespace std;

#define ANCHO_SPRITE 100
#define ALTO_SPRITE 100
#define TEMPO 30
/****************************************************************************************
 * 
 * 							Auxiliar
 *
 ****************************************************************************************/  

void imprimirMensaje (std::ostream &os, const std::string &msg, int num = NULL){
	os << msg << " : " << num << std::endl;
}




/****************************************************************************************
 * 
 * 							Auxiliar
 *
 ****************************************************************************************/  

void Personaje::personaje(int posicion_x, int posicion_y){
	
	this->posicion_x = posicion_x;
	this->posicion_y = posicion_y;
	this->estado = -1;
	this->modo = 0;
	this->lastTime = 0;
	//this->url = url;
	//this->imagenes = NULL;

	
	
}
 
/**Cada vez que se comience una nueva accion, se carga en un vector de punteros a SDL_Texture, 
 * las imagenes correspondientes a la misma.
 */
void Personaje::inicializar_movimiento_personaje(int estado, SDL_Renderer* ren){
		
	this->estado = estado; 
	this->modo = 0;
	this->lastTime = SDL_GetTicks();
	
	const std::string nombreCarpeta = std::to_string(estado);
	
	this->imagenes[0] = IMG_LoadTexture (ren, (nombreCarpeta+"/01.png").c_str());
	this->imagenes[1] = IMG_LoadTexture (ren, (nombreCarpeta+"/02.png").c_str());
	this->imagenes[2] = IMG_LoadTexture (ren, (nombreCarpeta+"/03.png").c_str());
	this->imagenes[3] = IMG_LoadTexture (ren, (nombreCarpeta+"/04.png").c_str());
	this->imagenes[4] = IMG_LoadTexture (ren, (nombreCarpeta+"/05.png").c_str());
	this->imagenes[5] = IMG_LoadTexture (ren, (nombreCarpeta+"/06.png").c_str());
	this->imagenes[6] = IMG_LoadTexture (ren, (nombreCarpeta+"/07.png").c_str());
	this->imagenes[7] = IMG_LoadTexture (ren, (nombreCarpeta+"/08.png").c_str());
	this->imagenes[8] = IMG_LoadTexture (ren, (nombreCarpeta+"/09.png").c_str());
	imprimirMensaje(std::cout, "Se cargaron las imagenes");	
}
void Personaje::reiniciar(){
	
	this->modo = 0;
	this->estado = -1;
	this->lastTime = 0;
	for (int i = 0; i < MAX_NUM_CUADROS; i++){
	//	SDL_DestroyTexture(imagenes[i]);
	}	
	
}
/**Se encarga de determinar segun el tiempo transcurrido, qué imagen corresponde
 * de la secuencia y la devuelve
 */ 
SDL_Texture* Personaje::definir_imagen(int nuevaAccion,SDL_Renderer* ren){
	
		
	int currentTime,tiempoTranscurrido;
	SDL_Texture* imagen_actual;
	
	/*La accion que se esta representando no coincide con la nuevaAccion
	 * pasada por parametro
	 * El loop del juego esperaba que representara una nuevaAccion
	 * Si el juego recien se inicia, incluso entonces la nuevaAccion no coincide con el estado 
	 * inicil del Personaje
	 */
	if (this->estado != nuevaAccion){
		/*Se deben inicializar el vector de imagenes correspondientes a la secuencia
		 */
		Personaje::reiniciar();
		Personaje::inicializar_movimiento_personaje(nuevaAccion,ren);
		imagen_actual = this->imagenes[0];
		
	}
	/*Se desea continuar con el mismo movimiento
	 */
	else{
		imprimirMensaje(std::cout,"Continuo con la accion");
		currentTime = SDL_GetTicks();
		imprimirMensaje(std::cout,"Modo",this->modo);
		imprimirMensaje(std::cout,"Tiempo actual",currentTime);
		imprimirMensaje(std::cout,"Tiempo pasado",this->lastTime);
		
		tiempoTranscurrido = currentTime - this->lastTime;
		imprimirMensaje(std::cout,"Tiempo transcurrido",tiempoTranscurrido);
		/*Debo actualizar la imagen a mostrar
		 */ 
		if (tiempoTranscurrido > TEMPO){
				
				if (this->modo < 8){
					
					this->modo = this->modo + 1;
					this->lastTime = this->lastTime + TEMPO;
					imagen_actual = this->imagenes[this->modo];
					//Personaje::cambiar_posicion(posicion_x,posicion_y);
					
				}
				/*Estoy en la misma accion que antes
				 *pero llegue al final de la secuencia de imagenes
				 * Debo volver al principio y mostrar la imagen 01
				 */ 
				else if (this->modo == 8){
				
						imprimirMensaje(std::cout, "LLegue al octavo estado");
						this->modo = 0;
						this->lastTime = this->lastTime + TEMPO;
						imagen_actual = this->imagenes[this->modo];
						
				}
		}
		else{
			/*Mantengo la imagen
			 * ergo: no hago nada
			 */ 
		}
	}
	
	return imagen_actual;
}

void Personaje::cambiar_posicion(int cant_pasos_x,int cant_pasos_y){
	
	
}
	
void Personaje::mirar_al_otro_lado(){
	
	
	}
