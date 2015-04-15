#ifndef CAMINARDERECHA_H_INCLUDED
#define CAMINARDERECHA_H_INCLUDED

#include "Accion.hpp"

class CaminarDerecha: public Accion{
	public:
		CaminarDerecha(string ruta, SDL_Renderer* ren):Accion(1,ruta,ren){};
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
		void execute(){
			if(secuenciaInversa){
				CaminarDerecha::cambiarModoInversamente();
			}
			else{
				Accion::cambiarModo();
			}
		};
		
};

#endif
