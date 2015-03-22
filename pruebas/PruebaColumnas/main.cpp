#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;

//Pantalla
unsigned int ANCHO_FISICO = 800;
unsigned int ALTO_FISICO = 416;
//Mundo
double AnchoLogico, AltoLogico;
SDL_Renderer * renderer = NULL;

//----------------------------------------
//Declaraciones.
void CargarEscenario(string nombre);
void CargarEscenarioDefault();
bool CargarPNG(string ruta);
//----------------------------------------

int main(int argc, char* args[]){
    //Nombre por parametro de consola
    //Busca archivo sii hay UN SOLO parametro.
    if (argc == 2){
        CargarEscenario(args[1]);
    }else{
        CargarEscenario("");
    }

	SDL_Window* ventana = NULL;
	//SDL_Surface* screenSurface = NULL;
	renderer = SDL_CreateRenderer(ventana, -1, 0);

	bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window * window = SDL_CreateWindow("Prueba1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ANCHO_FISICO, ALTO_FISICO, SDL_WINDOW_MAXIMIZED);

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_Surface * fondo = IMG_Load("fondo.png");
    SDL_Surface * columnasLejos = IMG_Load("columnasLejos.png");
    SDL_Surface * subZero = IMG_Load("subZeroFeliz0.png");

    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, fondo);
    SDL_Texture * textureSubZero = SDL_CreateTextureFromSurface(renderer, subZero);
    SDL_Texture * textureColumnasLejos = SDL_CreateTextureFromSurface(renderer, columnasLejos);

    IMG_Init(IMG_INIT_PNG);
    int moverX = 1;
    int derecha = 1;



    SDL_Event evento;
    while (!quit){
        SDL_PollEvent( &evento );
        switch(evento.type){

            case SDL_QUIT:
                quit = true;
                break;

            case SDL_KEYDOWN:
                if(evento.key.keysym.sym == SDLK_RIGHT){
                    derecha+= 5;
                }
                break;

       }

    //dibujo en pantalla
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_Rect rectanguloSZ = {0, 0 ,92,150};



      if (moverX <50){
      moverX++;
      rectanguloSZ.x = int(ANCHO_FISICO/2)-150+ moverX;
      }else{
      moverX=0;

        rectanguloSZ.x = int(ANCHO_FISICO/2)-150+ moverX;
      }



      rectanguloSZ.y = int(ALTO_FISICO/2)-92;

    SDL_Rect columnasLejosRCT = {0, 0 ,ANCHO_FISICO,ALTO_FISICO/2};
    columnasLejosRCT.x -= derecha;
    columnasLejosRCT.y  = (ALTO_FISICO/2.5);


    SDL_RenderCopy(renderer, textureColumnasLejos, NULL, &columnasLejosRCT);
    SDL_RenderCopy(renderer, textureSubZero, NULL, &rectanguloSZ);
    SDL_RenderPresent(renderer);
    }
    // LIBERAR RECURSOS
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(textureSubZero);
    SDL_DestroyTexture(textureColumnasLejos);
    SDL_FreeSurface(fondo);
    SDL_FreeSurface(subZero);
    SDL_FreeSurface(columnasLejos);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;

	/*//Initialize SDL
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ){
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}else{
		ventana = SDL_CreateWindow( "Prueba 1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ANCHO_FISICO, ALTO_FISICO, SDL_WINDOW_SHOWN );
		if( ventana == NULL ){
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		}else{

			//Get window surface
			screenSurface = SDL_GetWindowSurface( ventana );
			//Pintar de rojo
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 255, 0x00, 0x00 ) );


            CargarPNG("PepeLota.png");

			//Update the surface
			SDL_UpdateWindowSurface( ventana );
			//Esperar 2 segundos
			SDL_Delay( 1000 );
		}
	}






	SDL_DestroyWindow(ventana);
	IMG_Quit();
	SDL_Quit();
	return 0;*/
}
//--------------------------------------------------------------------------
void CargarEscenario(string nombre){
    cout<<"Archivo a buscar: "+ nombre<<endl;
    //bool ArchivoExiste = CargarArchivo("configuracion.json");

    //Hardcodeo 01
    bool ArchivoExiste;
    ArchivoExiste = false;
    //fin hardcodeo 01

    if(!ArchivoExiste){
        cout<<"No existe el .JSon"<<endl;
        CargarEscenarioDefault();
        return;
    }

    cout<<"Archivo encontrado"<<endl;
    //Lo de Martin.
}

//----------------------------------------------------------------------------
void CargarEscenarioDefault(){
    cout <<"Cargando default"<<endl;
}
