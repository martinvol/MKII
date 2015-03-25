#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "funcionesDibujar.h"
#include "cargadorDeEscenario.h"
using namespace std;

//Pantalla
unsigned int ANCHO_FISICO = 800; //800
unsigned int ALTO_FISICO = 416; //416

//Mundo
double AnchoLogico, AltoLogico;
SDL_Renderer * renderer = NULL;

//Capas
vector<SDL_Texture*> texturas;

//---------------------------------------------------
//-----------------------MAIN------------------------
//---------------------------------------------------
int main(int argc, char* args[]){
    if (InicializarSDL() != 0) return 1;


	SDL_Window* ventana = NULL;
	renderer = SDL_CreateRenderer(ventana, -1, 0);

	bool salir = false;

    SDL_Window * window = SDL_CreateWindow("Prueba1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ANCHO_FISICO, ALTO_FISICO, SDL_WINDOW_MAXIMIZED);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    //Nombre por parametro de consola
    //Busca archivo sii hay UN SOLO parametro.
    if (argc == 2){
        CargarEscenario(args[1], texturas,renderer);

    }else{
        CargarEscenarioDefault(texturas, renderer);
    }

    SDL_Texture *under = CargarTextura("p_under.png", renderer);
    SDL_Texture * fondo = CargarTextura("fondo.png", renderer);
    SDL_Texture * textureSubZero = CargarTextura("subZeroFeliz0.png", renderer);
    SDL_Texture *textureColumnasLejos = CargarTextura("columnasLejosFinal.png",renderer);
    SDL_Texture *textureColumnasMUYLejos = CargarTextura("columnasMUYLejos.png",renderer);
    SDL_Texture *floor= CargarTextura("p_1.png", renderer);

    //ESTO NO SE PUEDE--> MODIFICAR LAS IMAGENES.
    /*int altoDelPiso, anchoDelPiso =0;
    int anchoColumnasLejos, anchoColumnasMUYLejos =0;
    SDL_QueryTexture(floor, NULL, NULL, &anchoDelPiso, &altoDelPiso);
    SDL_QueryTexture(textureColumnasLejos, NULL, NULL, &anchoColumnasLejos, NULL);
    SDL_QueryTexture(textureColumnasMUYLejos, NULL, NULL, &anchoColumnasMUYLejos, NULL);*/


    int mover = 1;
    int moverSZ= 1;

    SDL_Event evento;
    while (!salir){
        SDL_PollEvent( &evento );
        switch(evento.type){
            case SDL_QUIT:
                salir = true;
                break;
            case SDL_KEYDOWN:
                if(evento.key.keysym.sym == SDLK_RIGHT)  {
                    if (abs(mover)<450) mover-= 5;
                    if (abs(-moverSZ)<450) moverSZ +=5;
                    /*cout<<"der "<<mover<<endl;
                    cout<<"ancho del piso: "<< anchoDelPiso<<endl;*/
                }
                if((evento.key.keysym.sym == SDLK_LEFT) && (mover <0) )  {
                    //cout<<"izq "<<mover<<endl;
                    mover+= 5;
                    moverSZ-=5;
                }
                if(evento.key.keysym.sym == SDLK_ESCAPE)  salir = true;
                break;
       }

        //dibujo en pantalla
        SDL_RenderClear(renderer);

        DibujarTextura(under, renderer, 0, 0, ANCHO_FISICO, ALTO_FISICO);
        DibujarTextura(fondo,renderer,0 ,0 ,ANCHO_FISICO,ALTO_FISICO);
        DibujarTextura(textureColumnasMUYLejos,renderer,0.5*mover ,0);
        DibujarTextura(textureColumnasLejos,renderer,mover,0);
        DibujarTextura(floor, renderer,0,0);
        //SubZero se OPONE al movimiento del fondo
        DibujarTextura(textureSubZero,renderer,15+moverSZ ,ALTO_FISICO-170);

        SDL_RenderPresent(renderer);
    }
    // LIBERAR RECURSOS
    // Pasar 'texturas', iterar, e ir eliminando cada una.
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

