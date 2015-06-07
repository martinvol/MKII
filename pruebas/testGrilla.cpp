#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cstdlib>
#include "../juego/Menu/GrillaDeJugadores.hpp"

using namespace std;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define NAME_CHAR "Mileena"

// Funciones auxiliares

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

/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @param renderer The renderer to load the texture in
* @return An SDL_Texture containing the rendered message, or NULL if something went wrong
*/
SDL_Texture* renderText(const string &message, const string &fontFile,
	SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
	//Open the font
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == NULL){
		logSDLError(cout, "TTF_OpenFont");
		return NULL;
	}	
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == NULL){
		TTF_CloseFont(font);
		logSDLError(cout, "TTF_RenderText");
		return NULL;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == NULL){
		logSDLError(cout, "CreateTexture");
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
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
	
	if (TTF_Init() != 0){
	    logSDLError(std::cout, "TTF_Init");
	    SDL_Quit();
	    return 1;
    }
	return 0;
}	


int main() {
	
	if (setUP() != 0) return 1;
	
	SDL_Window *win = SDL_CreateWindow("MK7542", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    if (win == NULL){
	    cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
	    SDL_Quit();
	    return 1;
    }
    
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED & SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL){
	    SDL_DestroyWindow(win);
	    cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
	    SDL_Quit();
	    return 1;
    }
    
    
    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Texture *image = renderText(NAME_CHAR, "resources/miscelaneo/Mk3.ttf", color, 9, renderer);
	
    //if (image == nullptr){
	//    cleanup(renderer, window);
	//    TTF_Quit();
	//    SDL_Quit();
	//    return 1;
	//}
    
    SDL_Texture *background = loadTexture("resources/background/p_bg.png", renderer);
    SDL_Texture *floor= loadTexture("resources/background/p_1.png", renderer);
    SDL_Texture *under = loadTexture("resources/background/p_under.png", renderer);
    SDL_Texture *lifeBar = loadTexture("resources/miscelaneo/01.gif", renderer);
    SDL_Texture *lifeBack = loadTexture("resources/miscelaneo/03.gif", renderer);
    SDL_Texture *timer = loadTexture("resources/miscelaneo/06.gif", renderer);
    //renderTexture(under, renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    Grilla* grilla = new Grilla(renderer);
    
    if (background == NULL || floor == NULL || under == NULL){
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
	    int ticks = SDL_GetTicks();
        int sprite = (ticks / 1000) % 10;
        int sprite2 = (ticks / 10000) % 10;
        SDL_Rect srcrect = { sprite * 13 - 3, 0, 13, 19 };
        SDL_Rect srcrect2 = { sprite2 * 13 - 3, 0, 13, 19};
        SDL_Rect dstrect2 = { 382, 10, 13, 19};
        SDL_Rect dstrect = { 400, 10, 13, 19 };
		while (SDL_PollEvent(&evento)){
		switch(evento.type){

		case SDL_QUIT:
			quit = true;
            break;
        case SDL_KEYDOWN:
            if(evento.key.keysym.sym == SDLK_RIGHT)	grilla->moverDerechaOpcion(0);
            if(evento.key.keysym.sym == SDLK_LEFT)  grilla->moverIzquierdaOpcion(0);
            if(evento.key.keysym.sym == SDLK_DOWN)  grilla->bajarOpcion(0);
            if(evento.key.keysym.sym == SDLK_UP) grilla->subirOpcion(0);
            if(evento.key.keysym.sym == SDLK_q) cout << grilla->seleccionarOpcion(0) << endl;
            if(evento.key.keysym.sym == SDLK_ESCAPE) quit = true;
            break;
		}
	}

        //dibujo en pantalla
        SDL_RenderClear(renderer);
        renderTexture(under, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        renderTexture(background, renderer, 1, 46*1.6 + 1, 400*1.6, 208*1.6);
        renderTexture(floor, renderer, derecha, SCREEN_HEIGHT - 46*1.6, 1216*1.6, 46*1.6);
        renderTexture(lifeBack, renderer, 0, 0);
        renderTexture(lifeBar, renderer, 0, 0, w, h);
        renderTexture(image, renderer, 0, 0);
        SDL_RenderCopy(renderer, timer, &srcrect, &dstrect);
        SDL_RenderCopy(renderer, timer, &srcrect2, &dstrect2);
        grilla->Dibujarse();
        SDL_RenderPresent(renderer);
        SDL_Delay(5);	
    }
    
	SDL_DestroyTexture(floor);
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(under);
    SDL_DestroyTexture(lifeBar);
    SDL_DestroyTexture(lifeBack);
    SDL_DestroyTexture(image);
    SDL_DestroyTexture(timer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    delete grilla;
    IMG_Quit();
    SDL_Quit();

	
}
