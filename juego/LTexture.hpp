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
		SDL_Texture* mTexture;
		LTexture(SDL_Renderer* ren);
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path, int hue_inicial, int hue_finale, int hue_offset);

		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Gets image dimensions
		int getWidth();
		int getHeight();

		//Pixel manipulators
		void* getPixels();
		int getPitch();
		
	private:
	    SDL_Renderer* gRenderer;
		int mWidth;
		int mHeight;
};


#endif // LTEX_H
