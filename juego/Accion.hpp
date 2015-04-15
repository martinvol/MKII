#ifndef ACCION_H_INCLUDED
#define ACCION_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

#include "parser.h"
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
		int lastTime;
		Conf* parser;
		Logger* logger;
		bool secuenciaInversa;

		Accion(int nroAccion, string ruta, SDL_Renderer* ren, Conf* parser); //constructor

		void setAccionNro(int nroAccion);
		void setRutaArchivo(const string directorio);
		void setModoActual(int modo);
		void setImagenes();
		void setRenderer(SDL_Renderer* ren);
		void setCantModos();
		
		void setInvertirSecuencia(){
			this->secuenciaInversa = true;
			this->setModoActual(this->cantModos-1);
		};
	
		SDL_Texture* getImagenActual();
		int getModoActual();
	
		bool esDistintaA(int nroAccion);
		bool esUltimoModo();
		void cambiarModo();
	
		virtual void execute(float tmp);
		virtual bool permite(int nuevaAccion){return true;};

		//SDL_Texture* getImagenNro(int numeroDeSprite);
		~Accion();
		virtual void resetear(){
			this->modoActual = 0;
			this->lastTime = 0;
			this->secuenciaInversa = false;
		}
		void cambiarModoInversamente();
};
#define TEMPO 30
class Quieto:public Accion{
	public:
		Quieto(string ruta, SDL_Renderer* ren,Conf* parser):Accion(0,ruta,ren,parser){};
		void execute(float tmp){
					
			Accion::cambiarModo();
			
		};
		bool permite(int nuevaAccion){
			return true;
		};	
};
class CaminarDerecha: public Accion{
	public:
		CaminarDerecha(string ruta, SDL_Renderer* ren, Conf* parser):Accion(1,ruta,ren, parser){};
		void cambiarModoInversamente(){
			if (this->modoActual==0){
				if(this->cantModos>0)
					setModoActual(this->cantModos-1);	
			}
			else{
				if (this->modoActual>0)
					setModoActual(this->modoActual-1);
			}
		
		};
		void execute(float tmp){
			if(secuenciaInversa){
				CaminarDerecha::cambiarModoInversamente();
			}
			else{
				Accion::cambiarModo();
			}
		};
		
};
#define TEMPOSALTO 150
class Saltar:public Accion{
	public:
		int contadorDeLoops;
		int contador;
		Saltar(string ruta, SDL_Renderer* ren, Conf* parser):Accion(3,ruta,ren, parser){this->contadorDeLoops=0;contador= 0;};
		void resetear(){
			this->modoActual = 0;
			this->contadorDeLoops=0;
			this->contador = 0;
		};
		void cambiarModo(){
			
			if (esUltimoModo()){
				if(this->cantModos>=3){///puts("entre aca");
					setModoActual(2);	
				}else
					setModoActual(0);
			}else{
				setModoActual(this->modoActual+1);
			}
			
		};
			
		void execute(float tmp){
			if (this->getModoActual() == 1){
				if(contadorDeLoops<21){
					contadorDeLoops+=1;
					return;
				}
			}
			Saltar::cambiarModo();
		};
};
class SaltarDiagonal: public Accion{
	public:
		SaltarDiagonal(string ruta, SDL_Renderer* ren, Conf* parser):Accion(4,ruta,ren, parser){};	
		void cambiarModo(){
			if (esUltimoModo()){
				///puts("entre aca");
				if(this->cantModos>=2)
					setModoActual(1);
				else
					setModoActual(0);
			}else{
				setModoActual(this->modoActual+1);
			}
		};
		void cambiarModoInversamente(){
			if(this->cantModos >= 2){
				if (this->modoActual==1){
					setModoActual(this->cantModos-1);	
				}
				else{
					setModoActual(this->modoActual-1);
				}
			}
		};
		void execute(float tmp){
			if(secuenciaInversa){
				SaltarDiagonal::cambiarModoInversamente();
			}
			else{
				SaltarDiagonal::cambiarModo();
			}
		};
		
};

#endif
