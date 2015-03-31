#include "ConversorDeCoordenadas.h"

ConversorDeCoordenadas::ConversorDeCoordenadas(float f_ancho, float f_alto, float x, float y){
	this->x_logico = x;
	this->y_logico = y;
	this->factor_ancho = f_ancho;
	this->factor_alto = f_alto;
}

CoordenadaFisica* ConversorDeCoordenadas::convertirAFisicas(CoordenadaLogica* coord){
	return new CoordenadaFisica((coord->x - x_logico)*factor_ancho, (coord->y - y_logico)*factor_alto);
}

CoordenadaLogica* ConversorDeCoordenadas::convertirALogicas(CoordenadaFisica* coord){
	return new CoordenadaLogica(x_logico + coord->x_fisico * 1.0 / factor_ancho, y_logico + coord->y_fisico * 1.0 / factor_alto);
}

void ConversorDeCoordenadas::seMovioVentana(int pixeles){
	x_logico += pixeles * 1.0 / factor_ancho;
}

void ConversorDeCoordenadas::seMovioVentana(float metros){
	x_logico += metros;
}
