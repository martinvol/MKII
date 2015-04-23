
#ifndef CAPA_H_INCLUDED
#define CAPA_H_INCLUDED

#include "../Coordenadas/ConversorDeCoordenadas.hpp"
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
		
		float ancho_logico_ventana;
		
		float b, m;
		
		
		Capa (string ubicacionParam, float anchoLogicoParam,  
			float x_logicoParam, SDL_Renderer *rendererParam, 
			float ancho_escenario, float ancho_ventana_logico);
		~Capa ();
		
		void DibujarseAnchoReal(int x, int y);
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
