#ifndef SALTARDIAGONAL_H_INCLUDED
#define SALTARDIAGONAL_H_INCLUDED

#include "Accion.hpp"

class SaltarDiagonal: public Accion{
	public:
		SaltarDiagonal(string ruta, SDL_Renderer* ren):Accion(4,ruta,ren){};	
		void cambiarModo(){
			if (esUltimoModo()){
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
		void execute(){
			if(secuenciaInversa){
				SaltarDiagonal::cambiarModoInversamente();
			}
			else{
				SaltarDiagonal::cambiarModo();
			}
		};
		
};

#endif
