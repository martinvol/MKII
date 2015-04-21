#include "CoordenadaLogica.hpp"

CoordenadaLogica::CoordenadaLogica(float x, float y){
	this->x = x;
	this->y = y;
}

CoordenadaLogica::CoordenadaLogica(CoordenadaLogica* coord){
	this->x = coord->x;
	this->y = coord->y;
}

void CoordenadaLogica::multiplicarPor(float f_x, float f_y){
	this->x *= f_x;
	this->y *= f_y;
}

void CoordenadaLogica::sumar(CoordenadaLogica* coord){
	this->x += coord->x;
	this->y += coord->y;
}

void CoordenadaLogica::desplazarX(float x){
	this->x += x;
}

void CoordenadaLogica::desplazarY(float y){
	this->y += y;
}

bool CoordenadaLogica::estaALaDerechaDe(CoordenadaLogica* coord){
	return (this->x > coord->x);
}

