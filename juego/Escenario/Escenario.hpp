#ifndef ESCENARIO_H_INCLUDED
#define ESCENARIO_H_INCLUDED

#include "Capa.hpp"
#include "CoordenadaFisica.hpp"


class Escenario{
public:
    
    float ancho;
    float alto;
    //Ordenado por z-index
    vector<Capa*> capas;
    
    
    Escenario(float ancho, float alto);
    // Se destruye con las capas que contiene.
    ~Escenario();

    // Libera todo
    void Borrar();

    //Se van agregando en orden por z-index
    void AgregarCapa(Capa *capaParam);

    //De Manu con puntero a funcion. Igual ver...
    //void Dibujar(CoordenadasFisicas Conversor(CoordenadasLogicas));
    void Dibujar(CoordenadaFisica* coordenadas);
    
    float obtenerLimiteDerecho();
    float obtenerLimiteIzquierdo();
    
    bool esLimiteDerecho(float x);
    bool esLimiteIzquierdo(float x);
};
#endif // ESCENARIO_H_INCLUDED
