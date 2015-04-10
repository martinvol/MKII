#include "ConversorDeCoordenadas.h"

// Se va a hacer:
// Si ALTO_FISICO --> ALTO_LOGICO,
// entonces, ANCHO_LOGICO --> ANCHO_FISICO * ALTO_LOGICO / ALTO_FISICO.

// La coordenada (0;0) física está en el extremo superior izquierdo de la ventana.
// Y la inicial lógica de la parte del escenario en pantalla, en el extremo inferior izquierdo de la ventana.

// Una vez que tengo los factores de conversión, si yo tengo una coordenada lógica (X;Y), su correspondiente física será:
// X_FISICA = (ANCHO_FISICO / ANCHO_LOGICO) * (X - x_logico)
// Y_FISICA = (ALTO_FISICO / ALTO_LOGICO) * (ALTO_LOGICO - Y)
// Por como hacemos, (ANCHO_FISICO / ANCHO_LOGICO) = (ALTO_FISICO / ALTO_LOGICO).

// Los atributos x e y lógicos se pueden inicializar en 0 para estar al principio del mundo o 0 y la mitad del ancho, como se desee.


ConversorDeCoordenadas::ConversorDeCoordenadas(int alto_fisico, int ancho_fisico, float alto_logico, float ancho_logico, float ancho_logico_ventana, float x){
	this->x_logico = x;
	this->alto_logico = alto_logico;
	this->ancho_logico = ancho_logico;
	this->factor_alto = (alto_fisico * 1.0) /( alto_logico * 1.0);
	this->factor_ancho = (ancho_fisico * 1.0)/(ancho_logico_ventana * 1.0);
	/*cout <<"ancho fisico" << conv->factor_alto << "\n"; ///
	cout <<"ancho logico" << conv->factor_alto << "\n"; ///*/
	this->alto_fisico = alto_fisico;
	this->ancho_fisico = ancho_fisico;
	this->ancho_logico_ventana = ancho_logico_ventana;
}

CoordenadaFisica* ConversorDeCoordenadas::aFisica(CoordenadaLogica* coord){
	int x_fisico = (coord->x - x_logico) * factor_ancho;
	int y_fisico = (alto_logico - coord->y)*factor_alto;
	return new CoordenadaFisica(x_fisico, y_fisico);
}

CoordenadaLogica* ConversorDeCoordenadas::aLogica(CoordenadaFisica* coord){
	return new CoordenadaLogica(x_logico + ((coord->x_fisico * 1.0)/factor_ancho), alto_logico - ((coord->y_fisico * 1.0)/factor_alto));
}

void ConversorDeCoordenadas::seMovioVentana(int pixeles){
	x_logico += pixeles * 1.0 / factor_ancho;
}

void ConversorDeCoordenadas::seMovioVentana(float metros){
	x_logico += metros;
}
