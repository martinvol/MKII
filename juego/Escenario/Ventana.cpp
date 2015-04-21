#include "Ventana.hpp"

Ventana::Ventana(char* titulo, int ancho_fisico, int alto_fisico, float margen){
	this->window = SDL_CreateWindow(titulo, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ancho_fisico, alto_fisico, SDL_WINDOW_MAXIMIZED);
	
	int ancho_de_borde = (ancho_fisico * (margen/100))*1.0 / 2;
	this->borde_izq = ancho_de_borde;
	this->borde_der = ancho_fisico - ancho_de_borde;
}

Ventana::~Ventana(){
	SDL_DestroyWindow(window);
}

void Ventana::cambiarTamaño(int ancho_fisico, int alto_fisico){
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

CoordenadaLogica* Ventana::obtenerBordeLogicoIzquierdo(ConversorDeCoordenadas* conv){
	coord_fis = new CoordenadaFisica(borde_izq, 0);
	CoordenadaLogica* coord_log = conv.aLogica(coord_fis);
	delete coord_fis;
	return coord_log;
}

CoordenadaLogica* Ventana::obtenerBordeLogicoDerecho(ConversorDeCoordenadas* conv){
	coord_fis = new CoordenadaFisica(borde_der, 0);
	CoordenadaLogica* coord_log = conv.aLogica(coord_fis);
	delete coord_fis;
	return coord_log;
}
