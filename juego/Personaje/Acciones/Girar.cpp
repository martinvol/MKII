#include "Girar.hpp"

using namespace std;
/***********************************************************************
 * 
 * 						CAMINAR
 * 
 **********************************************************************/


Girar::Girar(string ruta, SDL_Renderer* ren, SDL_Window* win, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(8,ruta,ren, win, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	//~ rectangulos->push_back(new Rectangulo(0, 50, 14, 22, false, false)); // Cabeza
	//~ rectangulos->push_back(new Rectangulo(20, 0, 30, 100, false, false)); // torso
	//~ rectangulos->push_back(new Rectangulo(49, 20, 50, 70, false, false)); // piernas
}

void Girar::cambiarModoInversamente(){
	if (this->modoActual==0){
	}
	else{
		if (this->modoActual>0)
			setModoActual(this->modoActual-1);
	}
}
void Girar::cambiarModo(){
	if (esUltimoModo()){	
	
	}else{
		setModoActual(this->modoActual+1);
	}

}

CoordenadaLogica* Girar::execute(CoordenadaLogica* coord_personaje){
	CoordenadaLogica* coord = new CoordenadaLogica(coord_personaje);
	coord->desplazarY(despl_y);
	if (secuenciaInversa){
		Girar::cambiarModoInversamente();
	} else {
		Girar::cambiarModo();
	}
	return coord;
};

bool Girar::permiteAccion(accion_posible nuevaAccion){
	if (Accion::esUltimoModo() and !secuenciaInversa){
		puts("putis");
		cout<<"modo actesfsfsgual"<<this->modoActual<<endl;
		return true;
	}
	if (modoActual == 0 and secuenciaInversa){
		puts("Hola");
		return true;
	}
	return false;
}
