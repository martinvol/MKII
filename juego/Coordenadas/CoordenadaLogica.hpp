#ifndef COORDLOGICA_H_INCLUDED
#define COORDLOGICA_H_INCLUDED
class CoordenadaLogica{
public:

    // Coordenadas en el mundo, en metros.
    float x;
    float y;

	CoordenadaLogica(float x, float y);
	CoordenadaLogica(CoordenadaLogica* coord);
	void multiplicarPor(float f_x, float f_y);
	void sumar(CoordenadaLogica* coord);
	void desplazarX(float x);
	void desplazarY(float y);
	bool estaALaDerechaDe(CoordenadaLogica* coord);

};

#endif // COORDLOGICA_H_INCLUDED
