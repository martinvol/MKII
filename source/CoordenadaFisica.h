#ifndef COORD_FIS_H_INCLUDED
#define COORD_FIS_H_INCLUDED
class CoordenadaFisica{

public:

    // Coordenadas en el mundo, en metros.
    int x_fisico;
    int y_fisico;

	CoordenadaFisica(int x, int y);
	void multiplicarPor(float f_x, float f_y);
	void sumar(CoordenadaFisica* coord);

};
#endif // COORDFISICA_H_INCLUDED
