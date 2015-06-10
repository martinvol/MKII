#include "Fatality.hpp"

Fatality::Fatality(Personaje* activo, Personaje* pasivo) {
	
	this->victima = pasivo;
	this->victimario = activo;
	this->desenmascararse = new Perform(ruta, ren, 0, 0, 0, this->victimario->accionActual->hue_init, this->victimario->accionActual->hue_fin, this->victimario->accionActual->hue_offset, 98);
	this->acido = new Perform(ruta, ren, 0, 0, 0, this->victimario->accionActual->hue_init, this->victimario->accionActual->hue_fin, this->victimario->accionActual->hue_offset, 99);
}

Fatality::~Fatality() {
	delete this->desenmascararse;
	delete this->acido;
}

Fatality::execute() {
	this->victimario->accionActual = desenmascararse; ///
	
}
