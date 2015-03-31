#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Personaje.h"
using namespace std;
const ANCHO_SPRITE 100;
const ALTO_SPRITE 100;
const int ESTADO_CAMINAR 1;
const int ESTADO_PARADO 0;
const int TEMPO 80;



void Personaje::crear(std::string url, posicion_x, posicion_y,){
	
	Personaje.posicion_x = posicion_x;
	Personaje.posicion_y = posicion_y;
	Personaje.estado = 0;
	Personaje.modo = 0;
	Personaje.lastTime = 0;
	Personaje.url = url;
	Personaje.imagenes = NULL;
	
}
 

void Personaje::inicializar_movimiento_personaje(int estado){
		
	Personaje.estado = estado; 
	Personaje.modo = 0;
	Personaje.lastTime = SDL_GetTicks();

	Personaje.imagenes[0] = IMG_Load ("01.png");
	Personaje.imagenes[1] = IMG_Load ("02.png");
	Personaje.imagenes[2] = IMG_Load ("03.png");
	Personaje.imagenes[3] = IMG_Load ("04.png");
	Personaje.imagenes[4] = IMG_Load ("05.png");
	Personaje.imagenes[5] = IMG_Load ("06.png");
	Personaje.imagenes[6] = IMG_Load ("07.png");
	Personaje.imagenes[7] = IMG_Load ("08.png");
	Personaje.imagenes[8] = IMG_Load ("09.png");
	
}
Personaje::reiniciar(){
	
	Personaje.modo = 0;
	Personaje.estado = 0;
	Personaje.lastTime = 0;
	for (int i = 0; i < MAX_NUM_CUADROS; i++){
		freeSurface(imagenes[i]);
	}	
	
}

SDL_Surface* Personaje::dibujar_imagen(int estado){
	
	unsigned int currentTime,tiempoTranscurrido;
	SDL_Surface* imagen_actual;
	
	if (Personaje.estado == 0){
	
		Personaje::inicializar_movimiento_personaje(estado);
		imagen_actual = Personaje.imagenes[0];
		
	}
	else{
		currentTime = SDL_GetTicks();
		tiempoTranscurrido = currentTime - Personaje.lastTime;
		
		if (tiempoTranscurrido > TEMPO){
				
				if (Personaje.modo < 8){
					
					Personaje.modo = Personaje.modo + 1;
					Personaje.lastTime = Personaje.lastTime + Tempo;
					imagen_actual = Personaje.imagenes[Personaje.modo];
					Personaje::cambiar_posicion(posicion_x,posicion_y);
					
					if (Personaje.modo == 8){
						Personaje::reiniciar();
					}
				}
				
		}
	}
	return imagen_actual;
}

void Personaje::cambiar_posicion(int cant_pasos_x,int cant_pasos_y){
	
	if (Director.puedoCambiarPosicion(Personaje.posicion_x,cant_pasos_x){
		Personaje.posicion_x += cant_pasos_x;
		Personaje.posicion_y += cant_pasos_y;
	}
}
