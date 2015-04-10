#ifndef COORDLOGICA_H_INCLUDED
#define COORDLOGICA_H_INCLUDED
class CoordenadaLogica{
public:

    // Coordenadas en el mundo, en metros.
    float x;
    float y;

	CoordenadaLogica(float x, float y);
	void multiplicarPor(float f_x, float f_y);
	// No libera la memoria de coord.
	void sumar(CoordenadaLogica* coord);
	void setearX(float x);
	void setearY(float y);
	void desplazarX(float x);
	void desplazarY(float y);
	// No libera la memoria de coord.
	bool estaALaDerechaDe(CoordenadaLogica* coord);

};

#endif // COORDLOGICA_H_INCLUDED
