// Wrapper obtenido de los Tutoriales de LazyFoo.
// http://lazyfoo.net/tutorials/SDL/40_texture_manipulation/index.php

/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

// Conversores obtenidos de StackOverFlow
// http://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both

#include "../juego/LTexture.hpp"
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <stdio.h>
#include <cmath>

using namespace std;

//RGB Default Transparent
Uint8 RT = 255;
Uint8 BT = 0;
Uint8 GT = 255;


typedef struct {
    double r;       // percent
    double g;       // percent
    double b;       // percent
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // percent
    double v;       // percent
} hsv;


double max3(double a, double b, double c) {
    if ((a >= b) && (a >= c)) return a;
    if ((b >= a) && (b >= c)) return b;
    return c;
}

double min3(double a, double b, double c) {
    if ((a <= b) && (a <= c)) return a;
    if ((b <= a) && (b <= c)) return b;
    return c;
}



hsv RGBaHSV(rgb in) {
// Taken from Wikipedia
    hsv out;
    
    double max = max3(in.r, in.g, in.b);
    double min = min3(in.r, in.g, in.b);
    
    if (min == max) out.h = NAN; 
     
    else if ((max == in.r) && (in.g >= in.b)) out.h = 60.0 * ((in.g - in.b) / (max - min)) + 0.0;
    else if ((max == in.r) && (in.g < in.b)) out.h = 60.0 * ((in.g - in.b) / (max - min)) + 360.0;
    else if (max == in.g) out.h = 60.0 * ((in.b - in.r) / (max - min)) + 120.0;
    else if (max == in.b) out.h = 60.0 * ((in.r - in.g) / (max - min)) + 240.0;
    
    if (max == 0) out.s = 0;
    else out.s = 1 - (min/max);
    
    out.v = max;
    
    return out;
}

void setHBetweenZeroAndTwoPi(double* h) {
    while ((*h < 0.0) || (*h >= 360.0)) *h += (-1)*(abs(*h)/(*h))*360.0;
}

rgb HSVaRGB(hsv in) {
// Tambien taken from Wiki

    rgb out;
    setHBetweenZeroAndTwoPi(&(in.h));
    
    long h_i = ((long)in.h) / 60; // Hay un mod 6 pero no le doy bola.
    double f = (in.h / 60.0) - h_i; // Tambien hay otro mod 6 pero tampoco le doy bola.
    
    double p = in.v * (1 - in.s);
    double q = in.v * (1 - f*(in.s));
    double t = in.v * (1 - (1 - f)*in.s);
    
    if (h_i == 0) {
        out.r = in.v;
        out.g = t;
        out.b = p;
    }
    else if (h_i == 1) {
        out.r = q;
        out.g = in.v;
        out.b = p;
    }
    else if (h_i == 2) {
        out.r = p;
        out.g = in.v;
        out.b = t;
    }
    else if (h_i == 3) {
        out.r = p;
        out.g = q;
        out.b = in.v;
    }
    else if (h_i == 4) {
        out.r = t;
        out.g = p;
        out.b = in.v;
    }
    else if (h_i == 5) {
        out.r = in.v;
        out.g = p;
        out.b = q;
    }
    
    return out;
}


LTexture::LTexture(SDL_Renderer* ren)
{
	//Initialize
	gRenderer = ren;
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	//~ free();
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText( string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	SDL_FreeSurface(loadedSurface);
	//Return success
	return mTexture != NULL;
}
#endif

bool LTexture::loadFromFile(std::string path, int hue_init, int hue_final, int hue_offset, bool espejar){

    hue_offset = (( hue_offset % 360 ) + 360 ) % 360;
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	//printf("Bits: %d \n", loadedSurface->format->BitsPerPixel);
	SDL_LockSurface(loadedSurface);
	
	Uint8 r, g, b, a;
	Uint32* pixels = (Uint32*) loadedSurface->pixels;
	int pixelCount = ( loadedSurface->pitch/4 ) * loadedSurface->h; 

    SDL_SetColorKey(loadedSurface,SDL_TRUE,SDL_MapRGB(loadedSurface->format,RT,BT,GT));

	for( int i = 0; i < pixelCount; ++i ) {
		SDL_GetRGBA(pixels[i], loadedSurface->format, &r, &g, &b, &a);
        // printf("r:%d g:%d b:%d\n", r, g, b);
        rgb in;
        in.r = r/255.0; in.g = g/255.0; in.b = b/255.0;
		//hsv out = rgb2hsv(in);
		hsv out = RGBaHSV(in);
        //printf("h:%f s:%f v:%f \n", out.h, out.s, out.v);
		if ((hue_init <= out.h) && (out.h <= hue_final)) { 
			out.h += hue_offset;
			if (out.h >= 360.0) out.h -= 360.0;
			//in = hsv2rgb(out);
            // printf("h:%f s:%f v:%f \n", out.h, out.s, out.v);
			in = HSVaRGB(out);
            r =in.r*255; g = in.g*255; b = in.b*255; 
            // r = (Uint8) in.r*255.; g = (Uint8) in.g*255.; b = (Uint8) in.b*255.; 
            // printf("r:%d g:%d b:%d\n", r, g, b);
            /*r = 0;
            g = 0;
            b = 0;*/
			pixels[i] = SDL_MapRGBA(loadedSurface->format, r, g, b, a);
            //puts("");
		}
	}
	
	if (espejar){
		int linea = loadedSurface->pitch/4;
		int columna = pixelCount/linea;
		
		//INVERTE PERFECTO
		//~ for(int i=0; i < pixelCount/2; i++){			
				//~ int x = pixels[pixelCount-1 -i];			
				//~ pixels[pixelCount-1 -i] =pixels[i];
				//~ pixels[i] = x;
		//~ }

		for(int i=0; i < pixelCount-linea; i+=linea){		
				for(int j=0; j< (linea/2); j++){
					int aux = pixels[i+j];
					pixels[i+j]= pixels[i+linea-j];
					pixels[i+linea-j] = aux;
				}			
		}
	}
	
	
	SDL_UnlockSurface(loadedSurface);
	mTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	SDL_QueryTexture(mTexture, NULL, NULL, &mWidth, &mHeight);
	SDL_FreeSurface(loadedSurface);
	devolverEspejado(mTexture);
	return true;	
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

//-------------------------------------------------------------------------
SDL_Texture* LTexture::devolverEspejado(SDL_Texture* original){
	return original;
	int w,h;
	SDL_QueryTexture(original, NULL, NULL, &w, &h);	
	
	void **pixels;
	int linea;
	SDL_LockTexture(original, NULL, pixels, &linea);
	int pixelCount = ( linea/4 ) * h; 
	
	Uint32* pixelsPosta = (Uint32*) pixels;
	
	
	
	for(int i=0; i < pixelCount-linea; i+=linea){		
		for(int j=0; j< linea/2; j++){
			int aux = pixelsPosta[i+j];
			pixelsPosta[i+j]= pixelsPosta[i+linea-j];
			pixelsPosta[i+linea-j] = aux;
		}			
	}
	//~ for( int i = 0; i < cantidadDePixels; i+=lineaDeCocucha ) {
		//~ for( int j = 0; j < lineaDeCocucha; j++) {
			//~ Uint32 temp = pixelsPosta[j];
			//~ pixelsPosta[i] = pixelsPosta[lineaDeCocucha -i];
			//~ pixelsPosta[lineaDeCocucha -i] = temp;			
		//~ }
	//~ }
	
	//~ cout<<lineaDeCocucha<<endl;
	SDL_UnlockTexture(original);
	return original;	
	
}
