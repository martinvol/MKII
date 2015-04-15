#ifndef SALTARVERTICAL_H_INCLUDED
#define SALTARVERTICAL_H_INCLUDED

#include "Accion.hpp"

class Saltar:public Accion{
	public:
		int contadorDeLoops;
		int contador;
		Saltar(string ruta, SDL_Renderer* ren):Accion(3,ruta,ren){this->contadorDeLoops=0;contador= 0;};
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
			
		void execute(){
			if (this->getModoActual() == 1){
				if(contadorDeLoops<21){
					contadorDeLoops+=1;
					return;
				}
			}
			Saltar::cambiarModo();
		};
};

#endif
