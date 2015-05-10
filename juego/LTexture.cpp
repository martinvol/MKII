// Wrapper obtenido de los Tutoriales de LazyFoo.
// http://lazyfoo.net/tutorials/SDL/40_texture_manipulation/index.php

/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/
#include "../juego/LTexture.hpp"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <stdio.h>

using namespace std;

LTexture::LTexture(SDL_Window* win, SDL_Renderer* ren)
{
	//Initialize
	gWindow = win;
	gRenderer = ren;
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	mPixels = NULL;
	mPitch = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		
		//Convert surface to display format
		gFormat = SDL_GetWindowSurface( gWindow )->format;
		SDL_Surface* formattedSurface = SDL_ConvertSurface( loadedSurface, gFormat, 0 );
		if( formattedSurface == NULL )
		{
			printf( "Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			
			
			//Create blank streamable texture
			newTexture = SDL_CreateTexture( gRenderer,SDL_GetWindowPixelFormat(gWindow), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h );
			if( newTexture == NULL )
			{
				printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
			}
			else
			{
				//Lock texture for manipulation
				SDL_LockTexture( newTexture, NULL, &mPixels, &mPitch );

				//Copy loaded/formatted surface pixels
				memcpy( mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h );

				//Unlock texture to update
				SDL_UnlockTexture( newTexture );
				mPixels = NULL;

				//Get image dimensions
				mWidth = formattedSurface->w;
				mHeight = formattedSurface->h;
			}

			//Get rid of old formatted surface
			SDL_FreeSurface( formattedSurface );
		}	
		
		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
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
		Uint8 alpha;
		SDL_GetSurfaceAlphaMod(loadedSurface, &alpha);
		SDL_SetTextureAlphaMod(mTexture, alpha);
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

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
		mPixels = NULL;
		mPitch = 0;
	}
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

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	this->actualizarTextura();
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

bool LTexture::lockTexture()
{
	bool success = true;

	//Texture is already locked
	if( mPixels != NULL )
	{
		printf( "Texture is already locked!\n" );
		success = false;
	}
	//Lock texture
	else
	{
		if( SDL_LockTexture( mTexture, NULL, &mPixels, &mPitch ) != 0 )
		{
			printf( "Unable to lock texture! %s\n", SDL_GetError() );
			success = false;
		}
	}

	return success;
}

bool LTexture::unlockTexture()
{
	bool success = true;

	//Texture is not locked
	if( mPixels == NULL )
	{
		printf( "Texture is not locked!\n" );
		success = false;
	}
	//Unlock texture
	else
	{
		SDL_UnlockTexture( mTexture );
		mPixels = NULL;
		mPitch = 0;
	}

	return success;
}

void* LTexture::getPixels()
{
	return mPixels;
}

int LTexture::getPitch()
{
	return mPitch;
}

float max3(float a, float b, float c) {
	
	if ((a >= b) && (a >= c)) return a;
	if ((b >= a) && (b >= c)) return b;
	return c;
}

float min3(float a, float b, float c) {
	// Las cosas que uno tiene que hacer cuando no esta en Python
	if ((a <= b) && (a <= c)) return a;
	if ((b <= a) && (b <= c)) return b;
	return c;
}

//~ void RGBaHSV(Uint8* r, Uint8* g, Uint8* b, float* h, float* s, float* v) {
	//~ // Recibe los punteros r, g, b. 'Devuelve' su transformada en
	//~ // HSV rellenando el contenido de los punteros.
	//~ 
	//~ // De Wikipedia:
	//~ 
	//~ float _r = *r / 255.0;
	//~ float _g = *g / 255.0;
	//~ float _b = *b / 255.0;
	//~ 
	//~ float max = max3(_r, _g, _b);
	//~ float min = min3(_r, _g, _b);
	//~ 
	//~ if (max == min) *h = nanf; // No esta definido
	//~ if ((max == _r) && (_g >= _b)) *h = 60* ((_g - _b)/(max - min));
	//~ else if ((max == _r) && (_g < _b))  *h = 60* ((_g - _b)/(max - min)) + 360;
	//~ else if (max == _g) *h = 60* ((_b - _r)/(max - min)) + 120;
	//~ else if (max == _b) *h = 60* ((_r - _g)/(max - min)) + 240;
	//~ 
	//~ // Para los demás no viene al caso aun
	//~ 
	//~ // H debe pertenecer a [0, 360)
	//~ *h = ((*h % 360) + 360) % 360;
	//~ *s = max==0 ? 0:(1 - (min/max));
	//~ *v = max;
//~ }
//~ 
//~ void HSVaRGB(float* h, float* s, float* v, Uint8* r, Uint8* g, Uint8* b) {
	//~ 
	//~ // De Wikipedia:
	//~ 
	//~ // H debe pertenecer a [0, 360)
	//~ *h = ((*h % 360) + 360) % 360;
	//~ 
	//~ if (isnan(*h) ){
		//~ *r = 0;
		//~ *g = 0;
		//~ *b = 0;
		//~ return;
	//~ }
	//~ 
	//~ int h_i = (*h / 60);
	//~ float f = (*h / 60.0) - h_i;
	//~ float p = (*v) * (1 - *s);
	//~ float q = (*v) * (1 - (*s)*f);
	//~ float t = (*v) * (1 - (1 - f)*(*s));
	//~ 
	//~ if (h_i == 0) {
		//~ *r = (*v)*255;
		//~ *g = t*255;
		//~ *b = p*255;
	//~ }
	//~ 
	//~ if (h_i == 1) {
		//~ *r = q*255;
		//~ *g = (*v)*255;
		//~ *b = t*255;
	//~ }
	//~ 
	//~ if (h_i == 2) {
		//~ *r = p*255;
		//~ *g = (*v)*255;
		//~ *b = t*255;
	//~ }
	//~ 
	//~ if (h_i == 3) {
		//~ *r = p*255;
		//~ *g = q*255;
		//~ *b = (*v)*255;
	//~ }
	//~ 
	//~ if (h_i == 4) {
		//~ *r = t*255;
		//~ *g = p*255;
		//~ *b = (*v)*255;
	//~ }
	//~ 
	//~ if (h_i == 5) {
		//~ *r = (*v)*255;
		//~ *g = p*255;
		//~ *b = q*255;
	//~ }
//~ }

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

    static hsv      rgb2hsv(rgb in);
    static rgb      hsv2rgb(hsv in);

hsv rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0              
            // s = 0, v is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}


rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;     
}


bool LTexture::modificarHue(int inicial, int finale, int offset) {
// Esta funcion recibe los valores indicados en el .json tales como
// se especifica en el enunciado.

	//~ //Recorrer los pixeles
	//~ //Obtener valores de R, G, B. (getRGB)
	//~ //Obtener valor en HSV/HSL
	//~ //Si esta en el rango, sumarle el offset
	//~ //Obtener valores R, G, B
	//~ //Obtener valor de pixel segun mapRGB
	//~ //Reemplazar el valor del pixel
	
	//Lock texture
	if(! this->lockTexture()) return false;

		Uint8 r, g, b, a;
		//int h, s, v;
		
		//Get pixel data
		Uint32* pixels = (Uint32*)this->getPixels();
		int pixelCount = ( this->getPitch() / 4 ) * this->getHeight(); // WTF ?

		for( int i = 0; i < pixelCount; ++i ) {
			SDL_GetRGBA(pixels[i], gFormat, &r, &g, &b, &a);
			rgb in;
			in.r = r/255.0; in.g = g/255.0; in.b = b/255.0;
			//RGBaHSV(&r, &g, &b, &h, &s, &v);
			hsv out = rgb2hsv(in);
			if ((inicial <= out.h) && (out.h <= finale)) { 
				out.h += offset;
				in = hsv2rgb(out);
				r = in.r*255; g = in.g*255; b = in.b*255; 
				//HSVaRGB(&h, &s, &v, &r, &g, &b);
				pixels[i] = SDL_MapRGBA(gFormat, r, g, b, a);
			}
		}


	this->actualizarTextura();
	//Unlock texture
	this->unlockTexture();
	return true;
}

int LTexture::actualizarTextura(){
	this->setBlendMode(SDL_BLENDMODE_BLEND);
	this->setAlpha(255);
	return (SDL_UpdateTexture(mTexture, NULL, this->getPixels(), this->getPitch()));
}
