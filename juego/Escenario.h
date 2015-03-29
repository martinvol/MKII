#ifndef ESCENARIO_H_INCLUDED
#define ESCENARIO_H_INCLUDED
class Escenario{
public:
    //Ordenado por z-index
    vector<Capa*> capas;

    // Libera todo
    void Borrar();

    //Se van agregando en orden por z-index
    void AgregarCapa(Capa *capaParam);

    //De Manu con puntero a funcion. Igual ver...
    //void Dibujar(CoordenadasFisicas Conversor(CoordenadasLogicas));
    void Dibujar(CoordenadasFisicas coordenadas);
};
#endif // ESCENARIO_H_INCLUDED