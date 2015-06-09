#include "GrillaDeJugadores.hpp"
#include "../LTexture.hpp"
#include <SDL2/SDL_ttf.h>

#include <iostream> ///

#define PATH1 "SubZero"
#define PATH2 "Ermac"
#define PATH3 "Rain"
#define PATH4 "NoobSaibot"
#define PATH5 "Scorpion"
#define PATH6 "Reptile"
#define PATH7 "Jade"
#define PATH8 "Cyrax"
#define PATH9 "Kitana"
#define PATH10 "Smoke_r"
#define PATH11 "Sektor"
#define PATH12 "Smoke"

#define BACKGROUND "resources/orangeportal.png"
#define OPCION "resources/menu/opcion_transparent.png"

#define ALWAYS_SUBZERO true

#define X_INIT 100
#define Y_INIT 120

#define HEADER_MSJ "ELIGE TU JUGADOR"
#define HEAD_X 100
#define HEAD_Y 0

#define CANT_ANCHO 4
#define TOTAL_IMAGENES 12

using namespace std;

#define JOYSTICK_DEAD_ZONE 10000

class ControladorGrilla{
	private:
	
	public:
		bool arriba, abajo, enter, en_boton;
		Grilla* menu;

ControladorGrilla(Grilla* menu){
	abajo = false;
	arriba = false;
	enter = false;
	this->menu = menu;
	en_boton = false;
}

void procesarEvento(SDL_Event* evento){
	//if ((evento->type == SDL_JOYAXISMOTION))	
	
	SDL_JoystickID numeroJoystick = (evento->jdevice.which);
	int jugador = int(numeroJoystick) % 2;
	while(SDL_PollEvent(evento)) {
		switch (evento->type){
			case SDL_JOYAXISMOTION:

			    /* If Up-Down movement */
				if( evento->jaxis.axis == 1){
					int y_Joystick = evento->jaxis.value;
					if( y_Joystick < -JOYSTICK_DEAD_ZONE ){ //  y = -1;		
						if (arriba == false) menu->subirOpcion(jugador);
						arriba = true;
					}else if( y_Joystick > JOYSTICK_DEAD_ZONE ){ //y =  1;		
						if (abajo == false) menu->bajarOpcion(jugador);
						abajo = true;
					}else{ //yDir = 0;
						arriba = false;
						abajo = false;
					}
				} else if (evento->jaxis.axis == 0) {
					int x_Joystick = evento->jaxis.value;
					if (x_Joystick < -JOYSTICK_DEAD_ZONE) menu->moverIzquierdaOpcion(jugador);
					else if(x_Joystick > JOYSTICK_DEAD_ZONE) menu->moverDerechaOpcion(jugador);
				}
				break;
			case SDL_KEYDOWN:
				if (evento->key.keysym.sym == SDLK_UP){
					if (arriba == false) menu->subirOpcion(jugador);
					arriba = true;
				} else
				if (evento->key.keysym.sym == SDLK_DOWN){
					if (abajo == false) menu->bajarOpcion(jugador);
					abajo = true;
				}
				if (evento->key.keysym.sym == SDLK_RETURN){
					enter = true;
				}
				break;
			case SDL_KEYUP:
				if (evento->key.keysym.sym == SDLK_UP){
					arriba = false;
				} else
				if (evento->key.keysym.sym == SDLK_DOWN){
					abajo = false;
				}
				break;
			case SDL_JOYBUTTONDOWN:
				if (evento->jbutton.button == 0){
					enter = true;
				}
				break;
			case SDL_MOUSEMOTION:
				if ((evento->motion.windowID == menu->idVentana) && (menu->entraEnGrilla(evento->motion.x, evento->motion.y))){
					//evento->motion.x
					int pos = menu->obtenerUbicacion(evento->motion.x, evento->motion.y);
					menu->xSeleccion[1] = (pos % (CANT_ANCHO))*menu->anchoImagen + menu->x_init;
					menu->ySeleccion[1] = (pos / (CANT_ANCHO))*menu->altoImagen + menu->y_init;	
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (evento->button.windowID == menu->idVentana){
					menu->seleccionarOpcion(1);
				}
				break;
			default:
				;
		}
	}
	if (enter == true){
		menu->seleccionarOpcion(jugador);
	}
	return;
}
};

SDL_Texture* texAPartirDeTexto(const string &message, const string &fontFile,
	SDL_Color color, int fontSize, SDL_Renderer *renderer) {
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}

int Grilla::obtenerUbicacion(int x, int y) {
	x -= this->x_init;
	y -= this->y_init;
	
	x /= this->anchoImagen;
	y /= this->altoImagen;
	
	return x + CANT_ANCHO*y;	
}

Grilla::Grilla(SDL_Renderer* renderer, int anchoVentana, int altoVentana) {
	this->ren = renderer;
	
	this->paths.push_back(PATH1);
	this->paths.push_back(PATH2);
	this->paths.push_back(PATH3);
	this->paths.push_back(PATH4);
	this->paths.push_back(PATH5);
	this->paths.push_back(PATH6);
	this->paths.push_back(PATH7);
	this->paths.push_back(PATH8);
	this->paths.push_back(PATH9);
	this->paths.push_back(PATH10);
	this->paths.push_back(PATH11);
	this->paths.push_back(PATH12);
	
	this->cargarTexturas();
	
	string pathBackground = BACKGROUND;
	this->background = IMG_LoadTexture(this->ren, pathBackground.c_str());
	
	this->anchoVentana = anchoVentana;
	this->altoVentana = altoVentana;
	
	this->x_init = this->anchoVentana/2 - (CANT_ANCHO*this->anchoImagen)/2;
	this->y_init = this->altoVentana/2 - ((TOTAL_IMAGENES/CANT_ANCHO)*this->altoImagen)/2;
	
	LTexture aux = LTexture(this->ren);
	
	aux.loadFromFile(OPCION, 0, 0, 0, false);
	this->seleccion.push_back(aux.mTexture);
	
	aux.loadFromFile(OPCION, 0, 60, 300, false);
	this->seleccion.push_back(aux.mTexture);
	
	this->xSeleccion.push_back(this->x_init);
	this->ySeleccion.push_back(this->y_init);
	this->xSeleccion.push_back(((CANT_ANCHO - 1)*this->anchoImagen) + this->x_init);
	this->ySeleccion.push_back(this->y_init);
	
	SDL_Color color = { 255, 255, 255, 255 };
    this->numero.push_back(texAPartirDeTexto("1", "resources/miscelaneo/Mk3.ttf", color, 32, renderer));
    this->numero.push_back(texAPartirDeTexto("2", "resources/miscelaneo/Mk3.ttf", color, 32, renderer));
    
    this->eligio.push_back(false);
    this->eligio.push_back(false);
    
    this->header = texAPartirDeTexto(HEADER_MSJ, "resources/miscelaneo/Mk3.ttf", color, 32, renderer); 
    int w, h;
	SDL_QueryTexture(this->header, NULL, NULL, &w, &h);
    this->x_header = this->anchoVentana/2 - w/2;
	this->y_header = HEAD_Y;
	
}

void Grilla::Dibujarse(){ 
	SDL_RenderClear(this->ren);	
	
	SDL_Rect rect;
	SDL_Rect head;
	SDL_QueryTexture(this->header, NULL, NULL, &(head.w), &(head.h));
	head.x = this->x_header;
	head.y = this->y_header;
	rect.w = this->anchoImagen;
	rect.h = this->altoImagen;
	
	SDL_RenderCopy(this->ren, this->background, NULL, NULL);
	SDL_RenderCopy(this->ren, this->header, NULL, &head);

	for(int i = 0; i < TOTAL_IMAGENES; i++) {
		rect.x = (i % CANT_ANCHO)*this->anchoImagen + this->x_init;
		rect.y = (i / CANT_ANCHO)*this->altoImagen + this->y_init;
		SDL_RenderCopy(this->ren, this->texs[i], NULL, &rect);
	}
	Uint32 ticks = SDL_GetTicks();
	rect.x = this->xSeleccion[0];
	rect.y = this->ySeleccion[0];
	SDL_Rect numerito = {rect.x, rect.y, rect.w/2, rect.h/2};
	int w, h;
	if ((ticks) % 7 == 0) this->accionesQuieto[this->obtenerUbicacion(this->xSeleccion[0], this->ySeleccion[0])]->cambiarModo();
	SDL_Texture* imagenJugador1 = this->accionesQuieto[this->obtenerUbicacion(this->xSeleccion[0], this->ySeleccion[0])]->getImagenActual(true);
	SDL_QueryTexture(imagenJugador1, NULL, NULL, &w, &h);
	SDL_Rect jugador1 = {0, 0, w, h};
	if (!this->eligio[0]) {
		SDL_RenderCopy(this->ren, this->seleccion[(ticks/100) % 2], NULL, &rect);
		SDL_RenderCopy(this->ren, this->numero[0], NULL, &numerito);
		SDL_RenderCopy(this->ren, imagenJugador1, NULL, &jugador1);				
	}
	rect.x = numerito.x = this->xSeleccion[1];
	rect.y = numerito.y = this->ySeleccion[1];
	if ((ticks) % 7 == 0) this->accionesQuieto[this->obtenerUbicacion(this->xSeleccion[1], this->ySeleccion[1])]->cambiarModo();
	SDL_Texture* imagenJugador2 = this->accionesQuieto[this->obtenerUbicacion(this->xSeleccion[1], this->ySeleccion[1])]->getImagenActual(false);
	SDL_QueryTexture(imagenJugador2, NULL, NULL, &w, &h);
	SDL_Rect jugador2 = {200, 0, w, h};
	if (!this->eligio[1]) {
		SDL_RenderCopy(this->ren, this->seleccion[(ticks/100) % 2], NULL, &rect);
		SDL_RenderCopy(this->ren, this->numero[1], NULL, &numerito);
		SDL_RenderCopy(this->ren, imagenJugador2, NULL, &jugador2);	
	}
	SDL_RenderPresent(this->ren);
}

void Grilla::cargarTexturas() {
	for(int i=0; i < this->paths.size(); i++) {
		string pathCompleto = "resources/jugador/"+this->paths[i]+"/"+this->paths[i]+".gif";
		this->texs.push_back(IMG_LoadTexture(this->ren, pathCompleto.c_str()));
		string pathJugador = "resources/jugador/"+this->paths[i]+"/";
		this->accionesQuieto.push_back(new Quieto(pathJugador, this->ren, 0, 0, 0, 0, 0, 0));
		//# this->accionesGanar.push_back(new Ganar(pathJugador, this->ren, 0, 0, 0, 0, 0, 0));
	}
	SDL_QueryTexture(this->texs[0], NULL, NULL, &this->anchoImagen, &this->altoImagen);
	this->anchoImagen *= 1.5;
	this->altoImagen *= 1.5;
}

Grilla::~Grilla() {
	for(int i = 0; i < this->texs.size(); i++) {
		SDL_DestroyTexture(this->texs[i]);
		delete this->accionesQuieto[i];
		//# delete this->accionesGanar[i];
	}
	for(int i = 0; i < this->seleccion.size(); i++) {
		SDL_DestroyTexture(this->seleccion[i]);
		SDL_DestroyTexture(this->numero[i]);
	}
	
	SDL_DestroyTexture(this->background);
	SDL_DestroyTexture(this->header);
}
string Grilla::obtenerPath(int jugador) {
	string name = this->paths[this->obtenerUbicacion(this->xSeleccion[jugador], this->ySeleccion[jugador])];
		
	return "resources/jugador/"+name+"/";
}

/*********************************************************************
 * ************************  CONTROLLER  *****************************
 * ******************************************************************/

void Grilla::subirOpcion(int jugador) {
	if (this->eligio[jugador]) return;
	
	this->ySeleccion[jugador] -= this->y_init;
	this->ySeleccion[jugador] -= this->altoImagen;
	this->ySeleccion[jugador] = (this->ySeleccion[jugador] + (TOTAL_IMAGENES/CANT_ANCHO)*this->altoImagen) 
								% (TOTAL_IMAGENES/CANT_ANCHO)*this->altoImagen;
	this->ySeleccion[jugador] += this->y_init;
}
void Grilla::bajarOpcion(int jugador) {
	if (this->eligio[jugador]) return;
	
	this->ySeleccion[jugador] -= this->y_init;
	this->ySeleccion[jugador] += this->altoImagen;
	this->ySeleccion[jugador] = (this->ySeleccion[jugador] + (TOTAL_IMAGENES/CANT_ANCHO)*this->altoImagen) 
								% (TOTAL_IMAGENES/CANT_ANCHO)*this->altoImagen;
	this->ySeleccion[jugador] += this->y_init;
}
void Grilla::moverDerechaOpcion(int jugador) {
	if (this->eligio[jugador]) return;
	
	this->xSeleccion[jugador] -= this->x_init;
	this->xSeleccion[jugador] += this->anchoImagen;
	if (this->xSeleccion[jugador] >= (CANT_ANCHO)*this->anchoImagen) this->xSeleccion[jugador] -= (CANT_ANCHO)*this->anchoImagen;
	this->xSeleccion[jugador] += this->x_init;
}
void Grilla::moverIzquierdaOpcion(int jugador) {
	if (this->eligio[jugador]) return;
	
	this->xSeleccion[jugador] -= this->x_init;
	this->xSeleccion[jugador] -= this->anchoImagen;
	if (this->xSeleccion[jugador] < 0) this->xSeleccion[jugador] += (CANT_ANCHO)*this->anchoImagen;
	this->xSeleccion[jugador] += this->x_init;
}

string Grilla::seleccionarOpcion(int jugador) {
	if(this->eligio[jugador]) return this->obtenerPath(jugador);
	
	this->eligio[jugador] = true;
	
	return this->obtenerPath(jugador);
}

void Grilla::open(Uint32 idVentana) {
	
	this->idVentana = idVentana;
	SDL_Event evento;
	this->Dibujarse();
	ControladorGrilla* controlador = new ControladorGrilla(this);
	while (!(this->eligio[0] && this->eligio[1])) {
	//while (!(this->eligio[0])) {
			controlador->procesarEvento(&evento);
			this->Dibujarse();
			//SDL_Delay(5);
				
	}
	delete controlador;
}

bool Grilla::entraEnGrilla(int x, int y) {
	return ((this->x_init <= x) && (x <= CANT_ANCHO*this->anchoImagen + this->x_init)
			&& (this->y_init <= y) && (y <= (TOTAL_IMAGENES/CANT_ANCHO)*this->altoImagen + this->y_init));
}

string Grilla::randomChoicePlayer2() {
	this->seleccionarOpcion(1);
	if (ALWAYS_SUBZERO) { 
		this->xSeleccion[1] = this->x_init;
		this->ySeleccion[1] = this->y_init;
	}
	else {
		//% Dar random de posicion x e y; hacer la misma conversion que 
		//% para el mouse.
	}
	return this->seleccionarOpcion(1);
} 
