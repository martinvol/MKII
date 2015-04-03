#ifndef CONVERSOR_H_INCLUDED
#define CONVERSOR_H_INCLUDED

#include "CoordenadaLogica.h"
#include "CoordenadaFisica.h"

class ConversorDeCoordenadas{

public:

	// Coordenada x del mundo real que ocupa actualmente el extremo izquierdo inferior de la ventana.
	// No se necesita la y porque es siempre igual.
    float x_logico;

	// Datos de la ventana
	float alto_logico;
	int alto_fisico;
	int ancho_fisico;

	// Factores de conversión para altura y ancho.
	float factor_ancho;
	float factor_alto;

	ConversorDeCoordenadas(int alto_fisico, int ancho_fisico, float alto_logico, float x);

	// Conversiones que crean nuevas coordenadas,y no destruyen viejas.
	
	CoordenadaFisica* convertirAFisicas(CoordenadaLogica* coord);
	/* Devuelve NULL si la coordenada lógica no debería estar en la pantalla. */
	
	CoordenadaLogica* convertirALogicas(CoordenadaFisica* coord);

	//Para avisarle que se movió la ventana dentro del mundo.
	void seMovioVentana(int pixeles);
	void seMovioVentana(float metros);
};
#endif // CONVERSOR_H_INCLUDED
