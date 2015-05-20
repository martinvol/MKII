#include "Gancho.hpp"

using namespace std;

/***********************************************************************
 * 
 * 						Gancho
 * 
 **********************************************************************/  

Gancho::Gancho(string ruta, SDL_Renderer* ren, float despl_x, float despl_y, float h_max, int hue_init, int hue_fin, int hue_offset):Accion(GANCHO,ruta,ren, despl_x, despl_y, h_max, hue_init, hue_fin, hue_offset){
	// (float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque, bool bloqueo)
	rectangulos->push_back(new Rectangulo(0, 45, 20, 22, false, false)); // Cabeza
	rectangulos->push_back(new Rectangulo(0, 70, 20, 30, true, false)); // piña
	rectangulos->push_back(new Rectangulo(20, 0, 30, 80, false, false)); // torso
	rectangulos->push_back(new Rectangulo(49, 10, 50, 70, false, false)); // piernas
};
///ESTO HACE QUE SOLO LO HAGA UNA VEZ NO IMPORTA QUE TANTO APRIETEN EL BOTON!
void Gancho::cambiarModoInversamente(){
	if (this->modoActual==0){
		if(this->cantModos>0)
			setModoActual(this->cantModos-1);	
			ciclos = 1;
	}
	else{
		if (this->modoActual>0)
			setModoActual(this->modoActual-1);
	}
}	
  	
void Gancho::cambiarModo(){
	if (modoActual==(cantModos-2)){
		setModoActual(0);
		setInvertirSecuencia();	
	}else{
		setModoActual(this->modoActual+1);
	}
}

CoordenadaLogica* Gancho::execute(CoordenadaLogica* coord_personaje){
	if(secuenciaInversa){
		Gancho::cambiarModoInversamente();	
	}
	else{
		Gancho::cambiarModo();
	}
	return new CoordenadaLogica(coord_personaje);
};

bool Gancho::permiteAccion(accion_posible nuevaAccion){
	return false;
}
