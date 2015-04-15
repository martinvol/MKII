#ifndef ACCION_H_INCLUDED
#define ACCION_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>


#include "logger.h"


using namespace std;

class Accion{
	
	private: 
		int cuentaArchivos(string ruta);
	public:
			
		int accionNro;
		int cantModos;
		string ruta;
		SDL_Renderer* renderer;
		vector<SDL_Texture*> imagenes;
		int modoActual;
		
		Logger* logger;
		bool secuenciaInversa;

		Accion(int nroAccion, string ruta, SDL_Renderer* ren); //constructor

		void setAccionNro(int nroAccion);
		void setRutaArchivo(const string directorio);
		void setModoActual(int modo);
		void setImagenes();
		void setRenderer(SDL_Renderer* ren);
		void setCantModos();
		
		void setInvertirSecuencia();
	
		SDL_Texture* getImagenActual();
		int getModoActual();
	
		bool esUltimoModo();
		void cambiarModo();
	
		virtual void execute();
		virtual bool permite(int nuevaAccion){return true;};

		~Accion();
		virtual void resetear();
		void cambiarModoInversamente();
};

#endif
