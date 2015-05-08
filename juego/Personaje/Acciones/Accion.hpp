#ifndef ACCION_H_INCLUDED
#define ACCION_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include "../../LTexture.hpp"

#include "../../Coordenadas/CoordenadaLogica.hpp"
#include "../../Logger/Logger.hpp"
#include "Rectangulo.hpp"

typedef enum accion_posible {QUIETO, CAMINAR_DERECHA, CAMINAR_IZQUIERDA, SALTAR, SALTARDIAGONAL_IZQ, SALTARDIAGONAL_DER, AGACHARSE} accion_posible;

using namespace std;

class Accion{
	
	private: 
		int cuentaArchivos(string ruta);
	public:
		
		/* Constantes de desplazamiento. */
		float despl_x;
		float despl_y;
		float h_max;
		
		bool direccionDerecha;
		
		int accionNro;
		int cantModos;
		string ruta;
		SDL_Renderer* renderer;
		SDL_Window* win;
		vector<LTexture*> imagenes;
		int modoActual;
		
		Logger* logger;
		bool secuenciaInversa;

		Accion(int nroAccion, string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max); //constructor

		void setAccionNro(int nroAccion);
		void setRutaArchivo(const string directorio);
		void setModoActual(int modo);
		void setImagenes();
		void setRenderer(SDL_Renderer* ren);
		void setCantModos();
		void setDireccionDerecha();
		void setDireccionIzquierda();
		void setInvertirSecuencia();
	
		LTexture* getImagenActual();
		int getModoActual();
	
		bool esUltimoModo();
		void cambiarModo();
		
		/* Devuelve una NUEVA coordenada. */
		virtual CoordenadaLogica* execute(CoordenadaLogica* coord_personaje){return NULL;}
		
		/* Devuelve true si puede ser interrumpida por alguna OTRA acción.
		 * Es decir, se supone que no le va a llegar la misma acción que es. */
		virtual bool permiteAccion(accion_posible nuevaAccion){return true;}

		std::vector<Rectangulo*>* rectangulos = NULL;

		~Accion();
		virtual void resetear();
		void cambiarModoInversamente();
};

#endif
