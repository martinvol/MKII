#include "Rectangulo.hpp"

using namespace std;

Rectangulo::Rectangulo(float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque){
	this->empieza_alto = empieza_alto;
	this->empieza_ancho = empieza_ancho;
	this->porcentaje_alto = porcentaje_alto;
	this->porcentaje_ancho = porcentaje_ancho;
	this->ataque = ataque;

}