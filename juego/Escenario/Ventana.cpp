#include "Ventana.hpp"

Ventana::Ventana(char* titulo, int ancho_fisico, int alto_fisico, float margen){
	
	this->window = SDL_CreateWindow(titulo, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ancho_fisico, alto_fisico, SDL_WINDOW_MAXIMIZED);
	
	int ancho_de_borde = (ancho_fisico * (1 - margen/100))*1.0 / 2;
	this->borde_izq = ancho_de_borde;
	this->borde_der = ancho_fisico - ancho_de_borde;
	this->alto_fisico = alto_fisico;
	this->ancho_fisico = ancho_fisico;
}

Ventana::~Ventana(){
	SDL_DestroyWindow(window);
}

void Ventana::cambiarTamano(int ancho_fisico, int alto_fisico){
	SDL_SetWindowSize(this->window, ancho_fisico, alto_fisico);
}

void Ventana::cambiarMargen(float margen){
	int ancho_de_borde = (ancho_fisico * (margen/100))*1.0 / 2;
	this->borde_izq = ancho_de_borde;
	this->borde_der = ancho_fisico - ancho_de_borde;
}

lugarEnVentana Ventana::coordenadaEnPantalla(CoordenadaFisica* coord){
	if (coord->x_fisico <= borde_izq) return bordeIzq;
	if (coord->x_fisico >= borde_der) return bordeDer;
	return medio;
}

float Ventana::obtenerMargenLogicoIzquierdo(ConversorDeCoordenadas* conv){
	CoordenadaFisica* coord_fis = new CoordenadaFisica(borde_izq, 0);
	CoordenadaLogica* coord_log = conv->aLogica(coord_fis);
	float margen = coord_log->x;
	delete coord_fis;
	delete coord_log;
	return margen;
}

float Ventana::obtenerMargenLogicoDerecho(ConversorDeCoordenadas* conv){
	CoordenadaFisica* coord_fis = new CoordenadaFisica(borde_der, 0);
	CoordenadaLogica* coord_log = conv->aLogica(coord_fis);
	float margen = coord_log->x;
	delete coord_fis;
	delete coord_log;
	return margen;
}

float Ventana::obtenerBordeLogicoIzquierdo(ConversorDeCoordenadas* conv){
	CoordenadaFisica* coord_fis = new CoordenadaFisica(0, 0);
	CoordenadaLogica* coord_log = conv->aLogica(coord_fis);
	float borde = coord_log->x;
	delete coord_fis;
	delete coord_log;
	return borde;
}

float Ventana::obtenerBordeLogicoDerecho(ConversorDeCoordenadas* conv){
	CoordenadaFisica* coord_fis = new CoordenadaFisica(ancho_fisico, 0);
	CoordenadaLogica* coord_log = conv->aLogica(coord_fis);
	float borde = coord_log->x;
	delete coord_fis;
	delete coord_log;
	return borde;
}

float Ventana::obtenerBordeSuperior(ConversorDeCoordenadas* conv){
	// En lo fisico, el y crece para abajo desde el techo. El techo es y = 0.
	CoordenadaFisica* coord_fis = new CoordenadaFisica(0, 0);
	CoordenadaLogica* coord_log = conv->aLogica(coord_fis);
	float borde = coord_log->y;
	delete coord_fis;
	delete coord_log;
	return borde;
}

bool Ventana::superaTecho(CoordenadaFisica* coord){
	return (coord->y_fisico < 0);
}
