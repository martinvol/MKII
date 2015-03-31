#ifndef CONVERSOR_H_INCLUDED
#define CONVERSOR_H_INCLUDED

#include "CoordenadaLogica.h"
#include "CoordenadaFisica.h"

class ConversorDeCoordenadas{

public:

	// Coordenadas del mundo real que ocupan actualmente el extremo izquierdo inferior de la ventana.
    float x_logico;
	float y_logico;

	// Factores de conversión para altura y ancho.
	float factor_ancho;
	float factor_alto;

	ConversorDeCoordenadas(float f_ancho, float f_alto, float x, float y);

	// Conversiones que crean nuevas coordenadas,y no destruyen viejas.
	CoordenadaFisica* convertirAFisicas(CoordenadaLogica* coord);
	CoordenadaLogica* convertirALogicas(CoordenadaFisica* coord);

	//Para avisarle que se movió la ventana dentro del mundo.
	void seMovioVentana(int pixeles);
	void seMovioVentana(float metros);
};
#endif // CONVERSOR_H_INCLUDED


// Se va a hacer:
// Si ALTO_FISICO = ALTO_LOGICO,
// entonces, ANCHO_LOGICO = ANCHO_FISICO * ALTO_LOGICO / ALTO_FISICO.

// Una vez que tengo los factores de conversión, si yo tengo una coordenada lógica (X;Y), su correspondiente física será:
// X_FISICA = (ANCHO_FISICO / ANCHO_LOGICO) * (X - x_logico)
// Y_FISICA = (ALTO_FISICO / ALTO_LOGICO) * (X - y_logico)
// Por como hacemos, (ANCHO_FISICO / ANCHO_LOGICO) = (ALTO_FISICO / ALTO_LOGICO).

// Los atributos x e y lógicos se pueden inicializar en 0 para estar al principio del mundo o 0 y la mitad del ancho, como se desee.

