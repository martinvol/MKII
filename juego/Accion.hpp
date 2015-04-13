#ifndef ACCION_H_INCLUDED
#define ACCION_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

#include "Parser.hpp"
#include "Logger.hpp"


using namespace std;

class Accion{
	
	
	public:
			
		int accionNro;
		int cantModos;
		string ruta;
		SDL_Renderer* renderer;
		vector<SDL_Texture*> imagenes;
		int modoActual;
		int lastTime;
		Parser* parser;
		Logger* logger;

		Accion(int nroAccion, string ruta, SDL_Renderer* ren, Parser* parser); //constructor

		void setAccionNro(int nroAccion);
		void setRutaArchivo(const string directorio);
		void setModoActual(int modo);
		void setImagenes();
		void setRenderer(SDL_Renderer* ren);
		void setCantModos();
	
		SDL_Texture* getImagenActual();
		int getModoActual();
	
		bool esDistintaA(int nroAccion);
		bool esUltimoModo();
		void cambiarModo();
	
		virtual void execute(float tmp);
		virtual bool permite(int nuevaAccion){return true;};

		//SDL_Texture* getImagenNro(int numeroDeSprite);
		~Accion();
		void resetear(){
			this->modoActual = 0;
			this->lastTime = 0;
		}
	
};
#define TEMPO 30
class Quieto:public Accion{
	public:
		Quieto(string ruta, SDL_Renderer* ren,Parser* parser):Accion(0,ruta,ren,parser){};
		void execute(float tmp){
			unsigned int currentTime = SDL_GetTicks();
			unsigned int tiempoTranscurrido = tmp - lastTime;
			
			if (tiempoTranscurrido > TEMPO){
					
					Accion::cambiarModo();
					lastTime = lastTime + TEMPO;
			}			
		};
		bool permite(int nuevaAccion){
			return true;
		};	
};
class CaminarDerecha: public Accion{
	public:
		CaminarDerecha(string ruta, SDL_Renderer* ren, Parser* parser):Accion(1,ruta,ren, parser){};
		void execute(float tmp){
			unsigned int currentTime = SDL_GetTicks();
			unsigned int tiempoTranscurrido = tmp - lastTime;
			
			if (tiempoTranscurrido > TEMPO){
					
					Accion::cambiarModo();
					lastTime = lastTime + TEMPO;
			}			
		};
		bool permite(int nuevaAccion){
			return true;
		};
};
class CaminarIzquierda: public Accion{
	public:
		CaminarIzquierda(string ruta, SDL_Renderer* ren, Parser* parser):Accion(2,ruta,ren, parser){};
		void execute(float tmp){
			unsigned int currentTime = SDL_GetTicks();
			unsigned int tiempoTranscurrido = tmp - lastTime;
			
			if (tiempoTranscurrido > TEMPO){
					
					Accion::cambiarModo();
					lastTime = lastTime + TEMPO;
			}			
		};
		bool permite(int nuevaAccion){
			return true;
		};
};
#define TEMPOSALTO 150
class Saltar:public Accion{
	public:
		Saltar(string ruta, SDL_Renderer* ren, Parser* parser):Accion(3,ruta,ren, parser){};
		void execute(float tmp){
			unsigned int currentTime = SDL_GetTicks();
			unsigned int tiempoTranscurrido = tmp - lastTime;
			cout<<"imprimir saltom, modo actual: "<<Accion::getModoActual()<<endl;
			if (Accion::getModoActual()==1){
				if (tiempoTranscurrido>TEMPOSALTO){
					Accion::cambiarModo();
					lastTime = lastTime + TEMPOSALTO;
				}
			}
			else{
				if (tiempoTranscurrido > 50){		
						Accion::cambiarModo();
						lastTime = lastTime + TEMPO;
				}	
			}		
		};
		bool permite(int nuevaAccion){
			cout<<"no permite"<<endl;
			return false;
		};
};
class SaltarDiagonal: public Accion{
	public:
		SaltarDiagonal(string ruta, SDL_Renderer* ren, Parser* parser):Accion(4,ruta,ren, parser){};	
		void execute(float tmp){
			cout<<"imprimir salto diagonal"<<endl;
			unsigned int currentTime = SDL_GetTicks();
			unsigned int tiempoTranscurrido = tmp - lastTime;
			//~ if (Accion::getModoActual()==1){
				//~ if (tiempoTranscurrido>TEMPOSALTO){
					//~ Accion::cambiarModo();
					//~ lastTime = lastTime + TEMPOSALTO;
				//~ }
			//~ }
			if (tiempoTranscurrido > 10){
					Accion::cambiarModo();
					lastTime = lastTime + 10;
			}
			
		};
		bool permite(int nuevaAccion){
			return false;
		};
};

#endif
