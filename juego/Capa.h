
#ifndef CAPA_H_INCLUDED
#define CAPA_H_INCLUDED

#include "ConversorDeCoordenadas.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
using namespace std;

class Capa{
// private:
	public:
		SDL_Renderer *ren;
		string ubicacion;
		float anchoLogico;
		float x_logico;
		
		float b, m;

		Capa (string ubicacionParam, float anchoLogicoParam, float x_logicoParam, SDL_Renderer *rendererParam, ConversorDeCoordenadas* conversor);
		~Capa ();
		void Dibujarse(int x, int y);
		void Dibujarse(int x, int y, int alto, int ancho);
		void Dibujarse2(int x, int y, ConversorDeCoordenadas* conversor);
		void DibujarseAnchoReal(int x, int y, ConversorDeCoordenadas* conversor);
		void DibujarseAnchoReal2(int x, int y, ConversorDeCoordenadas* conversor);
		SDL_Texture* CargarTextura();

		SDL_Texture* textura;
		ConversorDeCoordenadas* conversor;
};

//----------------------------------------------------------------
//----------------------------------------------------------------
//--------------------CONTENEDORES DE COORDENADAS-----------------
//------------------------Dani despues dice como -----------------
//--------------------------se transforman------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
class CoordenadasFisicas{
public:
    int x_fisico;
    int y_fisico;
};
//----------------------------------------------------------------
class CoordenadasLogicas{
public:
    double x_logico;
    double y_logico;
};
#endif // CAPA_H_INCLUDED
