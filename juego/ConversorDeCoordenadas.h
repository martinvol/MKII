#ifndef CONVERSOR_H_INCLUDED
#define CONVERSOR_H_INCLUDED
class ConversorDeCoordenadas{
public:

	// Coordenadas del mundo real que ocupan actualmente el extremo izquierdo inferior de la ventana.
    float x_logico;
	float y_logico;

	// Factores de conversión para altura y ancho.
	float factor_ancho;
	float factor_alto;

	ConversorDeCoordenadas(float f_ancho, float f_alto, float x, float y);

	CoordenadasFisicas convertirAFisicas(CoordenadasLogicas coord);
	CoordenadasLogicas convertirALogicas(CoordenadasFisicas coord);

	//Para avisarle que se movió la ventana dentro del mundo.
	void seMovioVentana();
};
#endif // CONVERSOR_H_INCLUDED


// Se va a hacer:
// Si ALTO_FISICO = ALTO_LOGICO,
// entonces, ANCHO_LOGICO = ANCHO_FISICO * ALTO_LOGICO / ALTO_FISICO.

// Una vez que tengo los factores de conversión, si yo tengo una coordenada lógica (X;Y), su correspondiente física será:
// X_FISICA = (ANCHO_FISICO / ANCHO_LOGICO) * (X - x_logico)
// Y_FISICA = (ALTO_FISICO / ALTO_LOGICO) * (X - y_logico)
// Por como hacemos, (ANCHO_FISICO / ANCHO_LOGICO) = (ALTO_FISICO / ALTO_LOGICO).

