#include "Capa.hpp"
#include "Escenario.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

void Escenario::AgregarCapa(Capa *capaParam){
    this->capas.push_back(capaParam);
}

//----------------------------------------------------------------
void Escenario::Dibujar(CoordenadaFisica* coordenada){
    for (unsigned int i = 0; i<= capas.size()-1; i++){
        capas[i]->DibujarseAnchoReal(coordenada->x_fisico, coordenada->y_fisico);
    }
}

//----------------------------------------------------------------
void Escenario::Borrar(){
//elimino cada objeto dinamico 'capa'
//que llama a su destructor, eliminando su propio obj. dinamico (la SDL_texture)
    for (unsigned int i = 0; i < capas.size(); i++){
        delete capas[i];
    }
    capas.clear();

}

void Escenario::Temblar(unsigned int ticks) {
     for (unsigned int i = 0; i <= capas.size() -1; i++){
        this->capas[i]->Temblar(ticks);
    }
}
    
Escenario::~Escenario(){
    puts("se borra el escenario");
	Borrar();
}

Escenario::Escenario(float ancho, float alto){
	this->ancho = ancho;
	this->alto = alto;
}

bool Escenario::esLimiteDerecho(float x){
	return (x >= ancho);
}

bool Escenario::esLimiteIzquierdo(float x){
	return (x <= 0);
}

float Escenario::obtenerLimiteDerecho(){
	return ancho;
}

float Escenario::obtenerLimiteIzquierdo(){
	return 0;
}
