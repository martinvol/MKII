#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;

void ImprimirError(ostream &os, const string &mensaje){
	os << mensaje << " error: " << SDL_GetError() << endl;
}

//--------------------------------------------------------------------
SDL_Texture* CargarTextura(const string &file, SDL_Renderer *ren){
	SDL_Texture *textura = IMG_LoadTexture(ren, file.c_str());
	if (textura == NULL){
		ImprimirError(cout, "No se pudo cargar la textura");
	}
	return textura;
}

//--------------------------------------------------------------------
void DibujarTextura(SDL_Texture *tex, SDL_Renderer *ren, int x_fisico, int y_fisico, int w, int h){
	//Rectangulo destino
	SDL_Rect dst;
	dst.x = x_fisico;
	dst.y = y_fisico;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

//--------------------------------------------------------------------
void DibujarTextura(SDL_Texture *tex, SDL_Renderer *ren, int x_fisico, int y_fisico){
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	DibujarTextura(tex, ren, x_fisico, y_fisico, w, h);
}

//--------------------------------------------------------------------
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

//--------------------------------------------------------------------

