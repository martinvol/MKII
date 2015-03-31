#include "CoordenadaFisica.h"

CoordenadaFisica::CoordenadaFisica(int x, int y){
	this->x_fisico = x;
	this->y_fisico = y;
}

void CoordenadaFisica::multiplicarPor(float f_x, float f_y){
	this->x_fisico *= f_x;
	this->y_fisico *= f_y;
}

void CoordenadaFisica::sumar(CoordenadaFisica* coord){
	this->x_fisico += coord.x_fisico;
	this->y_fisico += coord.y_fisico;
}
