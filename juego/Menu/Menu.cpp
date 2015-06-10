#include "Menu.hpp"

/************************************************************
*	 						BOTON							*
************************************************************/
BotonMenu::BotonMenu(modo mode, int ancho_fisico, int alto_fisico, int x, int y, SDL_Renderer* renderer){
	this->renderer = renderer;
	//~ fondo = IMG_LoadTexture(renderer, "resources/menu/fondo_boton.png");
	fuente = TTF_OpenFont("resources/miscelaneo/Mk3.ttf", 35);
	color = {245, 208, 51, 10};
	SDL_Surface* superficie;
	if (mode == Pelea){
		superficie = TTF_RenderText_Solid(fuente, "2 jugadores", color);
	} else
	if (mode == Practica){
		superficie = TTF_RenderText_Solid(fuente, "Practica", color);
	} else
	//if (mode == CPU)
	{
		superficie = TTF_RenderText_Solid(fuente, "Jugador vs CPU", color);
	}
	texto = SDL_CreateTextureFromSurface(renderer, superficie);	
	SDL_FreeSurface(superficie);	
	
	this->destino.x = x;
	this->destino.y = y;
	this->destino.w = ancho_fisico;
	this->destino.h = alto_fisico;
}

BotonMenu::~BotonMenu(){
	//~ SDL_DestroyTexture(fondo);
	SDL_DestroyTexture(texto);
}

void BotonMenu::Dibujarse(SDL_Texture* seleccion){
	//~ SDL_RenderCopy(renderer, fondo, NULL, &destino);
	SDL_RenderCopy(renderer, texto, NULL, &destino);
	if (seleccion != NULL){
		SDL_RenderCopy(renderer, seleccion, NULL, &destino);
	}
}

SDL_Rect BotonMenu::obtenerPosicionEnVentana(){
	return destino;
}



/************************************************************
*	 						MENU							*
************************************************************/

Menu::Menu(SDL_Renderer* renderer, Ventana* ventana){
	this->renderer = renderer;
	fondo = IMG_LoadTexture(renderer, "resources/menu/fondo3.png");
	opcion = IMG_LoadTexture(renderer, "resources/menu/opcion_transparent.png");
	seleccion = IMG_LoadTexture(renderer, "resources/menu/seleccion_transparent.png");
	
	ancho_fisico = ventana->ancho_fisico;
	alto_fisico = ventana->alto_fisico;
	idVentana = SDL_GetWindowID(ventana->window);
	
	reiniciar();
	
	int ancho_fisico_botones = ancho_fisico / 2.0;
	int alto_fisico_botones = alto_fisico / 6.0;
	int offset_entre_botones = alto_fisico_botones / 3.0;
	int x = (ancho_fisico / 2.0) - (ancho_fisico_botones / 2.0);
	int y = alto_fisico - 2*alto_fisico_botones;
	botones.push_back(new BotonMenu(Pelea, ancho_fisico_botones, alto_fisico_botones, x, y, renderer));
	y = y - offset_entre_botones - alto_fisico_botones;
	botones.push_back(new BotonMenu(Practica, ancho_fisico_botones, alto_fisico_botones, x, y, renderer));
	y = y - offset_entre_botones - alto_fisico_botones;
	botones.push_back(new BotonMenu(CPU, ancho_fisico_botones, alto_fisico_botones, x, y, renderer));
}

void Menu::reiniciar(){
	modo_actual = CPU;
	seleccionado = false;
}

Menu::~Menu(){
	SDL_DestroyTexture(fondo);
	SDL_DestroyTexture(opcion);
	SDL_DestroyTexture(seleccion);
	for (unsigned int i = 0; i < botones.size(); i++){
		delete botones[i];
	}
	botones.clear();
}

void Menu::Dibujarse(){
	SDL_RenderCopy(renderer, fondo, NULL, NULL);
	for(int i=0;i<botones.size();i++){
		if (i == (int) modo_actual){
			if (seleccionado){
				botones[i]->Dibujarse(this->seleccion);
			} else {
				botones[i]->Dibujarse(this->opcion);
			}
		} else {
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

vector<SDL_Rect> Menu::obtenerPosicionesBotones(){
	vector<SDL_Rect> vector;
	for(int i=0;i<botones.size();i++){
		vector.push_back(botones[i]->obtenerPosicionEnVentana());
	}
	return vector;
}

SDL_Renderer* Menu::obtenerRenderer(){
	return renderer;
}

Uint32 Menu::obtenerIDventana(){
	return idVentana;
}

void Menu::apuntarAOpcion(modo mode){
	modo_actual = mode;
}
