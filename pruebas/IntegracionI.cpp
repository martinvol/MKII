#include <iostream>
#include <vector>
#include "../juego/Capa.h"
#include "../juego/Escenario.h"
#include "../juego/parser.h"
#include "../juego/ConversorDeCoordenadas.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "jsoncpp/json/json.h"

#define JSONPATH "pruebas/integracion.json"

void ImprimirError(ostream &os, const string &mensaje){
	os << mensaje <<endl;
}

SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == NULL){
		ImprimirError(cout, "LoadTexture");
	}
	return texture;
}

int InicializarSDL() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		ImprimirError(cout, "Error al iniciar SDL");
		return 1;
	}
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
	    ImprimirError(cout, "Error con inicializacion de imagenes");
	    SDL_Quit();
	    return 1;
	}
    return 0;
}
void reiniciarJuego() { return; }

int main() {
      
    if (InicializarSDL() != 0) return 1;
    Conf* conf = new Conf();
 
    conf->set_values(JSONPATH);
    unsigned int ANCHO_FISICO = conf->ventana_anchopx; 
    unsigned int ALTO_FISICO = conf->ventana_altopx; 

    SDL_Window* window = SDL_CreateWindow("Mortal Kombat 3 Ultimate", 
                                           SDL_WINDOWPOS_CENTERED, 
                                           SDL_WINDOWPOS_CENTERED, 
                                           ANCHO_FISICO, 
                                           ALTO_FISICO, 
                                           SDL_WINDOW_MAXIMIZED);
                                           
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    //Escenario* escenario = new Escenario();
    
    for (unsigned int i = 0; i < conf->capas_vector.size(); i++) conf->capas_vector[i]->ren = renderer;  
        
        /*escenario->AgregarCapa(conf->capas_vector[i]); 
        escenario->AgregarCapa(
                new Capa (conf->capas_vector[i]->ubicacion,
                conf->capas_vector[i]->anchoLogico,
                conf->capas_vector[i]->x_logico,
                renderer)
            );
    }*/
    
    int mover = 1;
    int moverSZ= 1;
    bool salir = false;
    SDL_Event evento;
    SDL_Texture *under = loadTexture("resources/background/p_under.png", renderer);
    SDL_Rect r = {0, 0, ALTO_FISICO, ANCHO_FISICO};
    while (!salir){
        SDL_PollEvent(&evento);
        
        switch(evento.type){
        case SDL_QUIT:
            salir = true;
            break;
        case SDL_KEYDOWN:
            if (evento.key.keysym.sym == SDLK_RIGHT)  {
               if (abs(mover)<450) mover-= 5;
               if (abs(-moverSZ)<450) moverSZ +=5;
            }
            if ((evento.key.keysym.sym == SDLK_LEFT) && (mover <0))  {        
                mover+= 5;
                moverSZ-=5;
            }
            salir = (evento.key.keysym.sym == SDLK_ESCAPE);
            if (evento.key.keysym.sym == SDLK_r) reiniciarJuego();
            break;
       }

       SDL_RenderClear(renderer);
       
       SDL_RenderCopy(renderer, under, NULL,&r);
       /*
       (conf->capas_vector[0])->Dibujarse(0,0, ALTO_FISICO,ANCHO_FISICO);
       (escenario->capas[1])->Dibujarse(0.5*mover ,0);
       (escenario->capas[2])->Dibujarse(mover,0);
       (escenario->capas[3])->Dibujarse(0,ALTO_FISICO-46);
       (escenario->capas[4])->Dibujarse(15+moverSZ ,ALTO_FISICO-170); 
        */    
       SDL_RenderPresent(renderer);
       SDL_Delay(10);
    }
    
    //escenario->Borrar();
    for (unsigned int i = 0; i < conf->capas_vector.size(); i++) delete conf->capas_vector[i];
    SDL_DestroyTexture(under);  
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;   
}
