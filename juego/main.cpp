#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
//#include "funcionesDibujar.h"
//#include "cargadorDeEscenario.h"
#include "Capa.h"
#include "Escenario.h"
#include "parser.h"
using namespace std;

void ImprimirError(ostream &os, const string &mensaje){
	os << mensaje <<endl;
}
//----------------------------------------------------------------

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
//----------------------------------------------------------------
//----------------------------------------------------------------
//Pantalla
unsigned int ANCHO_FISICO = 800; //800
unsigned int ALTO_FISICO = 416; //416
//Mundo
double AnchoLogico, AltoLogico;
SDL_Renderer * renderer = NULL;

//---------------------------------------------------
//-----------------------MAIN------------------------
//---------------------------------------------------

int main(int argc, char* args[]){
    Conf conf;
    // puts(argv[1]);
    conf.set_values(argv[1]);
    // Martin
    if (InicializarSDL() != 0) return 1;

	SDL_Window* ventana = NULL;
	renderer = SDL_CreateRenderer(ventana, -1, 0);

	bool salir = false;
    SDL_Window * window = SDL_CreateWindow("Mortal Kombat 3 Ultimate", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ANCHO_FISICO, ALTO_FISICO, SDL_WINDOW_MAXIMIZED);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    //Nombre por parametro de consola
    //Busca archivo sii hay UN SOLO parametro.
    //Martin
    if (argc == 2){
        //CargarEscenario(args[1], texturas,renderer);

    }else{
        //CargarEscenarioDefault(texturas, renderer);
    }

    Escenario escenario;
    /*
        Martin: Necesito el vector resultadoJson con las Capas cargadas
        Fijate en Capa.h
        Necesito ubicacion, anchologico y x_logico
        crea c/capa (con new) y metela en el vector resultadoJson


        for (unsigned int i = 0; i <= resultadoJson.size(); i++){
            escenario.AgregarCapa(resultadoJson[i]);
        }
        Con esto se reemplazaria el bloque de lineas que sigue.
        Nota: subZeroFeliz0 es una Capa conceptual en realidad.
        Va a ser Personaje despues, charlalo con Mile :P.
    */

    Capa *fondo = new Capa(string("juego/fondo.png"),0.0, 0,renderer);
    Capa *Sz = new Capa("juego/subZeroFeliz0.png",0, 0, renderer);
    Capa *ColumnasLejos = new Capa("juego/columnasLejosFinal.png",0,0, renderer);
    Capa *columnasMuyLejos = new Capa("juego/columnasMUYLejos.png",0,0, renderer);
    Capa *piso = new Capa("juego/p_1.png",0,0, renderer);
    escenario.AgregarCapa(fondo); // 0
    escenario.AgregarCapa(columnasMuyLejos); // 1
    escenario.AgregarCapa(ColumnasLejos); // 2
    escenario.AgregarCapa(piso);// 3
    escenario.AgregarCapa(Sz); // 4 -CONCEPTUAL-


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

        //Limpio y dibujo
        SDL_RenderClear(renderer);
//Dibujarse(int x, int y, int alto, int ancho){

        //fondo
        escenario.capas[0]->Dibujarse(0,0, ALTO_FISICO,ANCHO_FISICO);
        //CML
        escenario.capas[1]->Dibujarse(0.5*mover ,0);
        //CL
        escenario.capas[2]->Dibujarse(mover,0);
        //piso
        escenario.capas[3]->Dibujarse(0,ALTO_FISICO-46);
        //Sz
        escenario.capas[4]->Dibujarse(15+moverSZ ,ALTO_FISICO-170);


       /* fondo->Dibujarse(0 ,0 ,ALTO_FISICO,ANCHO_FISICO);
        columnasMuyLejos->Dibujarse(0.5*mover ,0);
        ColumnasLejos->Dibujarse(mover,0);
        piso->Dibujarse(0,ALTO_FISICO-46);
        Sz->Dibujarse(15+moverSZ ,ALTO_FISICO-170);*/

        SDL_RenderPresent(renderer);
    }
    // LIBERAR RECURSOS
    escenario.Borrar();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit(); SDL_Quit();
    return 0;
}
