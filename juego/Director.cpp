#include "Director.h"

void Director::analizar_multievento_de_un_jugador(movimiento* mov, movimiento lugar){
	if (*mov == Arriba){
		if (lugar == Derecha) *mov = ArribaDerecha;
		else if (lugar == Izquierda) *mov = ArribaIzquierda;
		else *mov = lugar;
	} else if (*mov == Derecha) {
		if (lugar == Arriba) *mov = ArribaDerecha;
		else *mov = lugar;
	} else if (*mov == Izquierda) {
		if (lugar == Arriba) *mov = ArribaIzquierda;
		else  *mov = lugar;
	} else *mov = lugar;
}


/* Siempre va a pasar por acá antes del actualizar porque si no apretan
 * nada le va a mandar Nada (que sería parar). */
void Director::seMuevePersonaje(num_jugador jugador, movimiento lugar){
	switch (jugador){
		case jugador1:
			analizar_multievento_de_un_jugador(&mov1, lugar);
			break;
		default: //jugador2
			analizar_multievento_de_un_jugador(&mov2, lugar);
			break;
	}
}

void Director::actualizar(){
	// Les dice que cambien la "imagen" y "comportamiento" a la que le
	// piden, si debe.
	informar_acciones();
	// Haya cambiado o no de acción, yo sólo debo verificar que no se
	// vaya del margen o debería scrollear y si no puedo decirle que se
	// quede en donde está de ancho, pero la imagen está bien que si
	// cambie igual. (Por eso se puede separar).
	// Verifica, saca una posición válida y se la informa para que se
	// mueva allí.
	verificar_movimientos();
	// Finalmente, ya movidos, verifica en donde están uno respecto del
	// otro y les cambia la dirección de la mirada.
	verificar_orientaciones();
}

void Director::informar_acciones(){
	informar_accion(mov1, jugadores[jugador1]);
	informar_accion(mov2, jugadores[jugador2]);
}

/* Le va a decir al Jugador/Personaje que le dijeron de hacer
 * cierta acción. Éste va a saber si ignorarlo porque está haciendo otra
 * acción o interrumpir la acción que estaba haciendo por esta. */
void Director::informar_accion(movimiento mov, Jugador* jugador){
	switch (mov){
		case Derecha:
			jugador->caminarDerecha();
			break;
		case Izquierda:
			jugador->caminarIzquierda();
			break;
		case Arriba:
			jugador->saltar();
			break;
		case ArribaDerecha:
			jugador->saltarDerecha();
			break;
		case ArribaIzquierda:
			jugador->saltarIzquierda();
			break;
		default: //case Nada:
			jugador->parar();
			break;
	}
}

/* Por ahora, sólo valida el movimiento de UN JUGADOR. */
void Director::verificar_movimientos(){
	// Verificar en cada uno si debería scrollear, o si debería quedarse donde está.
	CoordenadaLogica* coord1 = jugadores[jugador1]->obtenerSiguienteCoordenadaDerSup();
	CoordenadaFisica* coord1_fis = this->conversor->aFisica(coord1);
	// Verifica altura.
	if (this->ventana->superaTecho(coord1_fis)){
		coord1->setearY(this->ventana->obtenerBordeSuperior(this->conversor));
	}
	
	// Caso: scrollear a la derecha.
	if (this->ventana->coordenadaEnPantalla(coord1_fis) == bordeDer){
		scrollearDerecha();
		float margen_der = this->ventana->obtenerMargenLogicoDerecho(this->conversor);
		if (coord1->x > margen_der) coord1->setearX(margen_der);
		jugadores[jugador1]->moverseADerSup(coord1);		
	}
	
	// Caso: scrollear a la izquierda.
	delete coord1;
	delete coord1_fis;
	coord1 = jugadores[jugador1]->obtenerSiguienteCoordenadaIzqSup();
	coord1_fis = this->conversor->aFisica(coord1);
	// Verifica altura.
	if (this->ventana->superaTecho(coord1_fis)){
		coord1->setearY(this->ventana->obtenerBordeSuperior(this->conversor));
	}
	
	if (this->ventana->coordenadaEnPantalla(coord1_fis) == bordeIzq) {
		float margen_izq = this->ventana->obtenerMargenLogicoIzquierdo(this->conversor);
		if (coord1->x < margen_izq) coord1->setearX(margen_izq);
		jugadores[jugador1]->moverseAIzqSup(coord1);
	}
	
	// Caso: la posición era válida en ancho.
	jugadores[jugador1]->moverseAIzqSup(coord1);
}

bool Director::sePuedeScrollearDerecha(){
	float borde_der = this->ventana->obtenerBordeLogicoDerecho(this->conversor);
	bool sePuede = not this->escenario->esLimiteDerecho(borde_der);
	return sePuede;
}

bool Director::sePuedeScrollearIzquierda(){
	float borde_izq = this->ventana->obtenerBordeLogicoIzquierdo(this->conversor);
	bool sePuede = not this->escenario->esLimiteIzquierdo(borde_izq);
	return sePuede;
}

void Director::scrollearDerecha(){
	if (not this->sePuedeScrollearDerecha()) return;
	float borde_der = this->ventana->obtenerBordeLogicoDerecho(this->conversor);
	if (this->escenario->esLimiteDerecho(borde_der+float(FACTOR_SCROLL)))
		this->conversor->seMueveVentana(this->escenario->obtenerLimiteDerecho() - borde_der);
	else this->conversor->seMueveVentana(float(FACTOR_SCROLL));
}

void Director::scrollearIzquierda(){
	if (not this->sePuedeScrollearIzquierda()) return;
	float borde_izq = this->ventana->obtenerBordeLogicoIzquierdo(this->conversor);
	if (this->escenario->esLimiteIzquierdo(borde_izq-float(FACTOR_SCROLL)))
		this->conversor->seMueveVentana(this->escenario->obtenerLimiteIzquierdo() - borde_izq);
	else this->conversor->seMueveVentana(- float(FACTOR_SCROLL));
}



void Director::verificar_orientaciones(){
	CoordenadaLogica* coord1 = jugadores[jugador1]->obtenerCoordenadaIzqSup();
	CoordenadaLogica* coord2 = jugadores[jugador2]->obtenerCoordenadaIzqSup();
	if (coord1->estaALaDerechaDe(coord2)){
		jugadores[jugador1]->mirarParaIzquierda();
		jugadores[jugador2]->mirarParaDerecha();
	} else {
		jugadores[jugador1]->mirarParaDerecha();
		jugadores[jugador2]->mirarParaIzquierda();
	}
	delete coord1;
	delete coord2;
}

