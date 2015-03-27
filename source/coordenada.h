#include<vector>

#ifndef COORD_H
#define COORD_H

class Coordenada {
        vector<float> posicion;
    public:
        Coordenada(float x, float y, float z = 0);
        vector<float> obtenerPosicion();
        void desplazarX(float cantidad);
        void desplazarY(float cantidad);
}

#endif
