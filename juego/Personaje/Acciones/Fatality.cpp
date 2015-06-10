#include "Fatality.hpp"

Fatality::Fatality(Personaje* activo, Personaje* pasivo, string ruta) {
	
	this->victima = pasivo;
	this->victimario = activo;
	SDL_Renderer* ren = this->victimario->renderer;
	this->desenmascararse = new Perform(ruta, ren, 0, 0, 0, this->victimario->accionActual->hue_init, this->victimario->accionActual->hue_fin, this->victimario->accionActual->hue_offset, 98);
	this->acido = new Perform(ruta, ren, 0, 0, 0, this->victimario->accionActual->hue_init, this->victimario->accionActual->hue_fin, this->victimario->accionActual->hue_offset, 99);
}

Fatality::~Fatality() {
	delete this->desenmascararse;
	delete this->acido;
}

bool Fatality::execute() {
	//this->victimario->accionActual = this->desenmascararse; ///
	if (this->victimario->accionActual->esUltimoModo()) this->victimario->cambiarAccionA(HACER_FATALITY);
	this->victimario->accionActual->cambiarModo();
	this->texAcidoActual = this->acido->getImagenActual(this->victimario->mirarDerecha);
	int w, h;
	SDL_QueryTexture(this->texAcidoActual, NULL, NULL, &w, &h);
	SDL_Rect rect = {this->victima->x, this->victima->y, w, h};
	SDL_RenderCopy(this->victimario->renderer, this->texAcidoActual, NULL, &rect);
	//SDL_RenderClear(this->victimario->renderer);
	SDL_RenderPresent(this->victimario->renderer);
	if (this->acido->getModoActual() == 15) this->victima->cambiarAccionA(DESAPARECER);
	this->acido->execute(this->victima->coordenada);
	return (!(this->acido->esUltimoModo()));
}
