#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;
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
void CargarEscenario(string nombre);
void CargarEscenarioDefault();
bool CargarPNG(string ruta);
//----------------------------------------

int main(int argc, char* args[]){
if (setUP() != 0) return 1;
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
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window * window = SDL_CreateWindow("Prueba1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ANCHO_FISICO, ALTO_FISICO, SDL_WINDOW_MAXIMIZED);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    SDL_Texture *under = loadTexture("p_under.png", renderer);
    SDL_Texture * fondo = loadTexture("fondo.png", renderer);
    SDL_Texture * textureSubZero = loadTexture("subZeroFeliz0.png", renderer);
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

    SDL_Event evento;
    while (!quit){
        SDL_PollEvent( &evento );
        switch(evento.type){
            case SDL_QUIT:
                quit = true;
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
                //if(evento.key.keysym.sym == SDLK_LEFT)  derecha-= 5;
                if(evento.key.keysym.sym == SDLK_ESCAPE)  quit = true;
                break;
       }

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
