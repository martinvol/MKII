#include "CoordenadaLogica.h"

CoordenadaLogica::CoordenadaLogica(float x, float y){
	this->x = x;
	this->y = y;
}

void CoordenadaLogica::multiplicarPor(float f_x, float f_y){
	this->x *= f_x;
	this->y *= f_y;
}

void CoordenadaLogica::sumar(CoordenadaLogica* coord){
	this->x += coord.x;
	this->y += coord.y;
}
