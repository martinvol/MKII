#ifndef ESTADO_H_INCLUDED
#define ESTADO_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include "Accion.hpp"
#include "Parser.hpp"

class Estado{
	
	public:
	
		Quieto* quieto;
		CaminarDerecha* caminarder;
		CaminarIzquierda* caminarizq;
		Saltar* saltar;
		SaltarDiagonal* saltardiagonal;
		
		//Parser* parser;

		Estado(string ruta, SDL_Renderer* ren, Parser* parser){
			this->quieto = new Quieto(ruta, ren,parser);
			this->saltar = new Saltar(ruta, ren,parser);
			this->saltardiagonal = new SaltarDiagonal(ruta, ren,parser);
			this->caminarizq = new CaminarIzquierda(ruta, ren,parser);
			this->caminarder = new CaminarDerecha(ruta, ren,parser);
		};
		~Estado(){
			delete this->quieto;
			delete this->caminarder;
			delete this->caminarizq;
			delete this->saltar;
			delete this->saltardiagonal;
		}

};

#endif // ESTADO_H_INCLUDED
