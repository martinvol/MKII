#include "Menu.hpp"

#include <iostream>
/************************************************************
*	 						BOTON							*
************************************************************/
Boton::Boton(modo mode, int ancho_fisico, int alto_fisico, int x, int y, SDL_Renderer* renderer){
	this->renderer = renderer;
	if (mode == Pelea){
		boton = IMG_LoadTexture(renderer, "resources/menu/Pelea.png");
	} else
	if (mode == Practica){
		boton = IMG_LoadTexture(renderer, "resources/menu/Practica.png");
	} else
	//if (mode == CPU)
	{
		boton = IMG_LoadTexture(renderer, "resources/menu/CPU.png");
	}
	
	this->destino.x = x;
	this->destino.y = y;
	this->destino.w = ancho_fisico;
	this->destino.h = alto_fisico;
	
}

Boton::~Boton(){
	SDL_DestroyTexture(boton);
}

void Boton::Dibujarse(SDL_Texture* seleccion){
	SDL_RenderCopy(renderer, boton, &destino, NULL);
	if (seleccion != NULL){
		SDL_RenderCopy(renderer, seleccion, &destino, NULL);
	}
}


/************************************************************
*	 						MENU							*
************************************************************/

Menu::Menu(SDL_Renderer* renderer, int ancho_fis_ventana, int alto_fis_ventana){
	this->renderer = renderer;
	fondo = IMG_LoadTexture(renderer, "resources/menu/fondo.png");
	opcion = IMG_LoadTexture(renderer, "resources/menu/Opcion.png");
	seleccion = IMG_LoadTexture(renderer, "resources/menu/Seleccion.png");
	
	ancho_fisico = ancho_fis_ventana;
	alto_fisico = alto_fis_ventana;
	modo_actual = CPU;
	seleccionado = false;
	
	int ancho_fisico_botones = ancho_fis_ventana / 2.0;
	int alto_fisico_botones = alto_fis_ventana / 6.0;
	int offset_entre_botones = alto_fisico_botones / 3.0;
	int x = (ancho_fis_ventana / 2.0) - (ancho_fisico_botones / 2.0);
	int y = alto_fis_ventana - 2*alto_fisico_botones;
	cout << "Ancho,Alto pantalla: " << ancho_fisico << "," << alto_fisico << endl;
	cout << "Primer par: " << x << "," << y << endl;
	botones.push_back(new Boton(Pelea, ancho_fisico_botones, alto_fisico_botones, x, y, renderer));
	y = y - offset_entre_botones - alto_fisico_botones;
	cout << "Segundo par: " << x << "," << y << endl;
	botones.push_back(new Boton(Practica, ancho_fisico_botones, alto_fisico_botones, x, y, renderer));
	y = y - offset_entre_botones - alto_fisico_botones;
	cout << "Tercer par: " << x << "," << y << endl;
	botones.push_back(new Boton(CPU, ancho_fisico_botones, alto_fisico_botones, x, y, renderer));
}

Menu::~Menu(){
	SDL_DestroyTexture(fondo);
	SDL_DestroyTexture(opcion);
	SDL_DestroyTexture(seleccion);
}

void Menu::Dibujarse(){
	SDL_RenderCopy(renderer, fondo, NULL, NULL);
	for(int i=0;i<botones.size();i++){
		if (i == (int) modo_actual){
			//~ cout << "ENTRO para: " << i << endl;
			if (seleccionado){
				botones[i]->Dibujarse(this->seleccion);
			} else {
				botones[i]->Dibujarse(this->opcion);
			}
		} else {
			//~ cout << "NO ENTRO para: " << i << endl;
			botones[i]->Dibujarse(NULL);
		}
	}
}

void Menu::subirOpcion(){
	modo_actual = (modo) ((modo_actual+1) % 3);
}

void Menu::bajarOpcion(){
	// Porque -1%3 = -1
	if (modo_actual == Pelea){
		modo_actual = CPU;
	} else {
		modo_actual = (modo) ((modo_actual-1) % 3);
	}
}

modo Menu::seleccionarOpcion(){
	seleccionado = true;
	return modo_actual;
}
