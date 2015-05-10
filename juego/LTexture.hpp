// Wrapper obtenido de los Tutoriales de LazyFoo.
// http://lazyfoo.net/tutorials/SDL/40_texture_manipulation/index.php

#ifndef LTEX_H
#define LTEX_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <stdio.h>

/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

class LTexture {
	public:
		//Initializes variables
		LTexture(SDL_Window* win, SDL_Renderer* ren);
		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, 
		SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

		//Pixel manipulators
		bool lockTexture();
		bool unlockTexture();
		void* getPixels();
		int getPitch();

		bool modificarHue(int inicial, int finale, int offset);
		
	private:
	    // Auxiliar
	    SDL_Window* gWindow;
	    SDL_Renderer* gRenderer;
	    SDL_PixelFormat* gFormat;
	    
		//The actual hardware texture
		SDL_Texture* mTexture;
		void* mPixels;
		int mPitch;

		//Image dimensions
		int mWidth;
		int mHeight;
		
		int actualizarTextura();
};


#endif // LTEX_H
