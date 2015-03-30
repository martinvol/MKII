#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <thread>         
#include <chrono> 
#include "Personaje.hpp"
using namespace std;

#define MAX_NUM_CUADROS 9
#define QUIETO 0
#define CAMINAR_DERECHA 1
#define CAMINAR_IZQUIERDA 2
#define SALTAR 3

/****************************************************************************************
 * 
 * 							Auxiliar
 *
 ****************************************************************************************/  

void imprimirMensajeE (std::ostream &os, const std::string &msg, int num = NULL){
	os << msg << " : " << num << std::endl;
}

/****************************************************************************************
 * 
 * 							Auxiliar
 *
 ****************************************************************************************/  


//-------------------------------------------------------------------
//-------------------------------------------------------------------
void logSDLError(ostream &os, const string &msg){
	os << msg << " error: " << SDL_GetError() << endl;
}

SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == NULL){
		logSDLError(cout, "LoadTexture");
	}
	return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}
int setUP() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logSDLError(cout, "SDLInitError");
		return 1;
	}
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
	    logSDLError(cout, "IMG_Init");
	    SDL_Quit();
	    return 1;
	}
	return 0;
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//Pantalla
unsigned int ANCHO_FISICO = 800; //800
unsigned int ALTO_FISICO = 416; //416
//Mundo
double AnchoLogico, AltoLogico;
SDL_Renderer * renderer = NULL;

//----------------------------------------
//Declaraciones.
bool CargarPNG(string ruta);
//----------------------------------------
void definir_paisaje (){
	
	
}
int main(int argc, char* args[]){
	
   
	
	Personaje personaje;
	
	bool quit = false;
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window * window = SDL_CreateWindow("Prueba1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ANCHO_FISICO, ALTO_FISICO, SDL_WINDOW_MAXIMIZED);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    SDL_Texture *under = loadTexture("p_under.png", renderer);
    SDL_Texture * fondo = loadTexture("fondo.png", renderer);
    SDL_Texture * textureSubZero = loadTexture("0/01.png", renderer);
    //SDL_Texture * textureSubZero;
    SDL_Texture *textureColumnasLejos = loadTexture("columnasLejosFinal.png",renderer);
    SDL_Texture *textureColumnasMUYLejos = loadTexture("columnasMUYLejos.png",renderer);
    SDL_Texture *floor= loadTexture("p_1.png", renderer);
    int altoDelPiso, anchoDelPiso =0;
    int anchoColumnasLejos, anchoColumnasMUYLejos =0;
    SDL_QueryTexture(floor, NULL, NULL, &anchoDelPiso, &altoDelPiso);
    SDL_QueryTexture(textureColumnasLejos, NULL, NULL, &anchoColumnasLejos, NULL);
    SDL_QueryTexture(textureColumnasMUYLejos, NULL, NULL, &anchoColumnasMUYLejos, NULL);


    int mover = 1;
    int moverSZ= 1;
	personaje.personaje(1,1);
	int i = 0;
    SDL_Event evento;
    
    /**LOOP DEL JUEGO*/
    
    while (!quit){
        
        imprimirMensajeE(std::cout,"-----------------------------------------------------------");
        //std::this_thread::sleep_for (std::chrono::seconds(1));
        
        while(SDL_PollEvent( &evento )){
        
			if (evento.type == SDL_QUIT){
					imprimirMensajeE(std::cout,"QUIT");
					quit = true;
					
			}
			else if (evento.type == SDL_KEYDOWN){
					/**MOVIMIENTO A LA DERECHA**/
					if(evento.key.keysym.sym == SDLK_RIGHT)  {
						imprimirMensajeE(std::cout,"DERECHA");
						textureSubZero = personaje.definir_imagen(CAMINAR_DERECHA,renderer); 
					}
					/**MOVIMIENTO A LA IZQUIERDA**/
					else if(evento.key.keysym.sym == SDLK_LEFT) {
						textureSubZero = personaje.definir_imagen(CAMINAR_IZQUIERDA,renderer);
						}
					
					else if(evento.key.keysym.sym == SDLK_ESCAPE) { 
						imprimirMensajeE(std::cout,"ESCAPE");
						quit = true;
					}
			}
			/*else{
						imprimirMensajeE(std::cout,"DEFAULT");
						textureSubZero = personaje.definir_imagen(QUIETO,renderer);
			}*/	
       }
	
		imprimirMensajeE(std::cout,"DEFAULT");
		textureSubZero = personaje.definir_imagen(QUIETO,renderer);
		//dibujo en pantalla
	
		SDL_RenderClear(renderer);

		renderTexture(under, renderer, 0, 0, ANCHO_FISICO, ALTO_FISICO);
		renderTexture(fondo,renderer,0 ,0 ,ANCHO_FISICO,ALTO_FISICO-altoDelPiso);

		renderTexture(textureColumnasMUYLejos,renderer,0.5*mover ,0);
		//renderTexture(textureColumnasMUYLejos,renderer,0.5*mover ,0, ANCHO_FISICO, ALTO_FISICO );
		renderTexture(textureColumnasMUYLejos,renderer,0.5*mover+anchoColumnasMUYLejos+(anchoColumnasMUYLejos/5) ,(ALTO_FISICO/2.5));
		renderTexture(textureColumnasLejos,renderer,2*mover ,(ALTO_FISICO/2.5));
		renderTexture(textureColumnasLejos,renderer,2*mover +anchoColumnasLejos +(anchoColumnasLejos/10),(ALTO_FISICO/2.5));
		renderTexture(floor, renderer, mover, ALTO_FISICO - altoDelPiso);
		//SubZero se OPONE al movimiento del fondo
		renderTexture(textureSubZero,renderer,15+moverSZ ,ALTO_FISICO-170);

		SDL_RenderPresent(renderer);
		SDL_Delay(1000/40.);
    }
    // LIBERAR RECURSOS
    SDL_DestroyTexture(under);
    SDL_DestroyTexture(fondo);
    SDL_DestroyTexture(textureSubZero);
    SDL_DestroyTexture(textureColumnasLejos);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
