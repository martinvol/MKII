#include "Fatality.hpp"

Fatality::Fatality(Personaje* activo, Personaje* pasivo, string ruta) {
	
	this->victima = pasivo;
	this->victimario = activo;
	SDL_Renderer* ren = this->victimario->renderer;
	this->desenmascararse = new Perform(ruta, ren, 0, 0, 0, this->victimario->accionActual->hue_init, this->victimario->accionActual->hue_fin, this->victimario->accionActual->hue_offset, 98);
	this->acido = new Perform(ruta, ren, 0, 0, 0, this->victimario->accionActual->hue_init, this->victimario->accionActual->hue_fin, this->victimario->accionActual->hue_offset, 99);
	this->anchoVictima = 0;
	this->altoVictima = 0;
	this->x = -1;
	this->y = -1;
	this->anchoPrimerAcido = this->altoPrimerAcido = 0;
}

Fatality::~Fatality() {
	delete this->desenmascararse;
	delete this->acido;
}

bool Fatality::execute() {
	//this->victimario->accionActual = this->desenmascararse; ///
	this->victimario->activarAccion(HACER_FATALITY);
	
	if(this->victima->nroAccionActual != DESAPARECER) this->victima->cambiarAccionA(DIZZY);
	if (!this->victimario->accionActual->esUltimoModo()) {
		this->victimario->coordenada = this->victimario->accionActual->execute(this->victimario->coordenada);
		return true;
	}
	this->acido->execute(NULL);
	this->texAcidoActual = this->acido->getImagenActual(this->victimario->mirarDerecha);
	int w, h;
	SDL_QueryTexture(this->texAcidoActual, NULL, NULL, &w, &h);
	
	if (this->x == -1) {
		this->anchoVictima = this->victima->ancho;
		this->altoVictima = this->victima->alto;
		this->x = this->victima->x;
		this->y = this->victima->y;	
	}
	if (this->acido->getModoActual() <= 15) {
		this->anchoPrimerAcido = w;
		this->altoPrimerAcido = h;	
	}
	if (this->acido->getModoActual() == 11) this->victima->cambiarAccionA(DESAPARECER);
	SDL_Rect rect = {this->x - w/(2*this->victima->conversor->factor_ancho), // + (this->anchoVictima - w)*this->victima->conversor->factor_ancho, 
					this->y + (this->altoPrimerAcido - h)*this->victima->conversor->factor_alto, 
					w*this->victima->conversor->factor_ancho, h*this->victima->conversor->factor_alto};
					
	SDL_RenderCopy(this->victimario->renderer, this->texAcidoActual, NULL, &rect);
	//SDL_RenderClear(this->victimario->renderer);
	SDL_RenderPresent(this->victimario->renderer);
	
	
	return (!(this->acido->esUltimoModo()));
}
