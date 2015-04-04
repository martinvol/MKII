#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

using namespace std;

class Accion{
	
	bool permiteInterrupcion;
	int accionNro;
	int cantModos;
	string ruta;
	SDL_Renderer* renderer;
	vector<SDL_Texture*> imagenes;
	int modoActual;
	public:
		
		Accion(int nroAccion, string ruta, bool permite,SDL_Renderer* ren); //constructor
		~Accion();
		void setAccionNro(int nroAccion);
		void setRutaArchivo(const string directorio);
		void setModoActual(int modo);
		void setImagenes();
		void setPermiteInterrupcion(bool permite);
		void setRenderer(SDL_Renderer* ren);
		void setCantModos();
		SDL_Texture* getImagenActual();
		bool esDistintaA(int nroAccion);
		bool esUltimoModo();
		void cambiarModo();
	
};
