#ifndef QUIETO_H_INCLUDED
#define QUIETO_H_INCLUDED

#include "Accion.hpp"

class Quieto:public Accion{
	public:
		Quieto(string ruta, SDL_Renderer* ren):Accion(0,ruta,ren){};
		void execute(){
					
			Accion::cambiarModo();
			
		};
		bool permite(int nuevaAccion){
			return true;
		};	
};


#endif
