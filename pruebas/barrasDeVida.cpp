#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cstdlib>

using namespace std;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Funciones auxiliares

void logSDLError(ostream &os, const string &msg){
	os << msg << " error: " << SDL_GetError() << endl;
}

SDL_Texture* loadTexture(const string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr){
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


int main() {
	
	if (setUP() != 0) return 1;
	
	SDL_Window *win = SDL_CreateWindow("MK7542", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    if (win == nullptr){
	    cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
	    SDL_Quit();
	    return 1;
    }
    
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
	    SDL_DestroyWindow(win);
	    cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
	    SDL_Quit();
	    return 1;
    }
    
    SDL_Texture *background = loadTexture("resources/background/p_bg.png", renderer);
    SDL_Texture *floor= loadTexture("resources/background/p_1.png", renderer);
    SDL_Texture *under = loadTexture("resources/background/p_under.png", renderer);
    SDL_Texture *lifeBar = loadTexture("resources/miscelaneo/01.gif", renderer);
    SDL_Texture *lifeBack = loadTexture("resources/miscelaneo/03.gif", renderer);
    //renderTexture(under, renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    if (background == nullptr || floor == nullptr || under == nullptr){
        SDL_DestroyRenderer(renderer); 
        SDL_DestroyWindow(win);
	    IMG_Quit();
	    SDL_Quit();
	    return 1;
    }
	SDL_Event evento;
	int derecha = 1;
	bool quit = false;
	int w, h;
    SDL_QueryTexture(lifeBar,NULL, NULL, &w, &h);
	while (!quit){
	    
		SDL_PollEvent(&evento);
		switch(evento.type){

		case SDL_QUIT:
			quit = true;
            break;
        case SDL_KEYDOWN:
            if(evento.key.keysym.sym == SDLK_RIGHT)	derecha+= 5;
            if(evento.key.keysym.sym == SDLK_LEFT)  derecha-= 5;
            if(evento.key.keysym.sym == SDLK_DOWN)  w -= 1;
            break;
		}
		

        //dibujo en pantalla
        SDL_RenderClear(renderer);
        renderTexture(under, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        renderTexture(background, renderer, 1, 46*1.6 + 1, 400*1.6, 208*1.6);
        renderTexture(floor, renderer, derecha, SCREEN_HEIGHT - 46*1.6, 1216*1.6, 46*1.6);
        renderTexture(lifeBack, renderer, 0, 0);
        renderTexture(lifeBar, renderer, 0, 0, w, h);   
        SDL_RenderPresent(renderer);
        SDL_Delay(1);	
    }
    
	SDL_DestroyTexture(floor);
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(under);
    SDL_DestroyTexture(lifeBar);
    SDL_DestroyTexture(lifeBack);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();

	
}
