#include <iostream>
#include <string>
#include <cstdlib>  
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

//Screen dimension constants
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
//----------------------------------------

//Declaraciones.
void CargarEscenario();
void CargarEscenarioDefault();
//----------------------------------------

//--------------------------------------------------------------------------
void CargarEscenario(){
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


/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(ostream &os, const string &msg){
	os << msg << " error: " << SDL_GetError() << endl;
}

/**
* Loads an image into a texture on the rendering device
* @param file The image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr){
		logSDLError(cout, "LoadTexture");
	}
	return texture;
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
* width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param w The width of the texture to draw
* @param h The height of the texture to draw
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}



int loadStage(int argc, char* args[]) {
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
	    cout << "SDL_Init Error: " << SDL_GetError() << endl;
	    return 1;
    }
    
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
	    logSDLError(cout, "IMG_Init");
	    SDL_Quit();
	    return 1;
    }   
    
    SDL_Window *win = SDL_CreateWindow("Mortal Kombat 7542", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    if (win == nullptr){
	    logSDLError(cout, "SDL_CreateWindow");
	    SDL_Quit();
	    return 1;
    }
    
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
	    SDL_DestroyWindow(win);
	    logSDLError(cout, "SDL_CreateRenderer");
	    SDL_Quit();
	    return 1;
    }
    
    
    SDL_Texture *background = loadTexture("resources/background/p_bg.png", renderer);
    SDL_Texture *image = loadTexture("resources/background/p_4.png", renderer);
    //Make sure they both loaded ok
    if (background == nullptr || image == nullptr){
        SDL_DestroyRenderer(renderer); 
        SDL_DestroyWindow(win);
	    IMG_Quit();
	    SDL_Quit();
	    return 1;
    }
    
    //Get window surface
    
	int w, h;
	SDL_QueryTexture(background, NULL, NULL, &w, &h);
    renderTexture(background, renderer,0, 30, w*1.61, h*1.61);
    SDL_QueryTexture(image, NULL, NULL, &w, &h);
    renderTexture(image, renderer,0, 120, w*1.61, h*1.61);
    SDL_Surface* screenSurface = SDL_GetWindowSurface(win);
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
			
	//Update the surface
	SDL_UpdateWindowSurface(win);
    SDL_RenderPresent(renderer);
    SDL_Delay(4000);
   
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(image);
    SDL_DestroyRenderer(renderer); 
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit(); 
   
    return 0;
}
