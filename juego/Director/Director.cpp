#include "Director.hpp"
#include "../Logger/Logger.hpp"

/*********************************************************************
 * 
 * 					CONSTRUCTOR Y DESTRUCTOR
 * 
 *********************************************************************/

Director::Director(Escenario* escenario, Ventana* ventana, ConversorDeCoordenadas* conversor, Personaje* personaje1, Personaje* personaje2, BarraDeVida* barra1, BarraDeVida* barra2, float factor_scroll, Timer* timer){
	this->escenario = escenario;
	this->ventana = ventana;
	this->conversor = conversor;
	jugadores.push_back(new Jugador(personaje1, barra1));
	jugadores.push_back(new Jugador(personaje2, barra2));
	this->factor_scroll = factor_scroll;
	this->timer = timer;
}

Director::~Director(){
	delete escenario;
	delete ventana;
	delete conversor;
	for (unsigned int i = 0; i <= jugadores.size() -1; i++){
        delete jugadores[i];
    }
    jugadores.clear();
    delete timer;
}

/*********************************************************************
 * 
 * 						AUXILIARES ṔRIVADAS
 * 
 *********************************************************************/


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
			jugador->activarAccion(CAMINAR_DERECHA);
			break;
		case Izquierda:
			jugador->activarAccion(CAMINAR_IZQUIERDA);
			break;
		case Arriba:
			jugador->activarAccion(SALTAR);
			break;
		case Abajo:
///FALTA IMPLEMENTAR AGACHARSE
			//~ case Abajo:
			//~ jugador->activarAccion(AGACHARSE);
			//~ break;	
			jugador->activarAccion(QUIETO);
			cout<<"agachado"<<endl; ///
			break;			
		case Pararse:
///FALTA IMPLEMENTAR PARARSE
			jugador->activarAccion(QUIETO);
			cout<<"parandose"<<endl; ///
			break;		
		case ArribaDerecha:
			jugador->activarAccion(SALTARDIAGONAL_DER);
			break;
		case ArribaIzquierda:
			jugador->activarAccion(SALTARDIAGONAL_IZQ);
			break;		
		case PiniaAlta:
			cout<<"pinia alta"<<endl; ///
			jugador->activarAccion(PINIAALTA);
			//Una vez que la ejecuto, la desactivo, sino loopea.
			(jugador->obtenerPersonaje())->PiniaAlta = false;
			break;
		case PiniaBaja:
			cout<<"pinia baja"<<endl; ///
			jugador->activarAccion(PINIABAJA);
			//Una vez que la ejecuto, la desactivo, sino loopea.
			(jugador->obtenerPersonaje())->PiniaBaja = false;
			break;
		case PatadaAlta:
			cout<<"PatadaAlta"<<endl; ///
			//jugador->activarAccion(PINIAALTA);
			//Una vez que la ejecuto, la desactivo, sino loopea.
			(jugador->obtenerPersonaje())->PatadaAlta = false;
			break;
		case PatadaBaja:
			cout<<"PatadaBaja"<<endl; ///
			//jugador->activarAccion(PINIAALTA);
			//Una vez que la ejecuto, la desactivo, sino loopea.
			(jugador->obtenerPersonaje())->PatadaBaja = false;
			break;
		case Cubrirse:
			cout<<"Cubrirse"<<endl; ///
			//jugador->activarAccion(PINIAALTA);
			//Una vez que la ejecuto, la desactivo, sino loopea.
			(jugador->obtenerPersonaje())->Cubrirse = false;
			break;
		case ArrojarArma:
			cout<<"ArrojarArma"<<endl; ///
			//jugador->activarAccion(PINIAALTA);
			//Una vez que la ejecuto, la desactivo, sino loopea.
			(jugador->obtenerPersonaje())->ArrojarArma = false;
			break;
		case MirarDerecha:
			puts("hola?"); ///
			jugador->activarAccion(MIRARDERECHA);
			break;
		case MirarIzquierda:
			jugador->activarAccion(MIRARIZQUIERDA);
			break;
		default: //case Nada:
			jugador->activarAccion(QUIETO);
			break;
	}
}

void Director::verificar_movimiento(Jugador* jugador, Jugador* elOtro){

	// Verificar en cada uno si debería scrollear, o si debería quedarse donde está.
	CoordenadaLogica* coordSig = jugador->obtenerSiguienteCoordenadaDerSup();
	CoordenadaFisica* coordSig_fis = this->conversor->aFisica(coordSig);
		// Verifica altura.
	if (this->ventana->superaTecho(coordSig_fis)){
		coordSig->setearY(this->ventana->obtenerBordeLogicoSuperior(this->conversor));
	}
	
	// Si se choca con el otro personaje, no se puede mover.
	// Lado Derecho de Este jugador:
	CoordenadaLogica* coordEste = jugador->obtenerCoordenadaDerInf();
	CoordenadaLogica* coordOtro = elOtro->obtenerCoordenadaIzqSup();
	  // Si estaba a la izquierda, y quiere terminar a la derecha, se choca y no puede.
	if ((coordEste->x <= coordOtro->x) && (coordSig->x >= coordOtro->x) && (coordSig->y <= coordOtro->y)){
		coordSig->setearX(coordOtro->x);
		jugador->moverseADerSup(coordSig);
		delete coordSig;
		delete coordSig_fis;
		delete coordEste;
		delete coordOtro;
		return;
	}
	delete coordEste;
	delete coordOtro;
	
	// Caso: scrollear a la derecha.
	if (this->ventana->coordenadaEnPantalla(coordSig_fis) == bordeDer){
		scrollearDerecha();
		float margen_der = this->ventana->obtenerMargenLogicoDerecho(this->conversor);
		if (coordSig->x > margen_der){
			coordSig->setearX(margen_der);
		}
		jugador->moverseADerSup(coordSig);
		delete coordSig;
		delete coordSig_fis;
		return;		
	}
	delete coordSig;
	delete coordSig_fis;
	
	// Caso: scrollear izquierda
	coordSig = jugador->obtenerSiguienteCoordenadaIzqSup();
	coordSig_fis = this->conversor->aFisica(coordSig);
		// Verifica altura.
	if (this->ventana->superaTecho(coordSig_fis)){
		coordSig->setearY(this->ventana->obtenerBordeLogicoSuperior(this->conversor));
	}
	
	// Si se choca con el otro personaje, no se puede mover.
	// Lado Izquierdo de Este jugador:
	coordEste = jugador->obtenerCoordenadaIzqInf();
	coordOtro = elOtro->obtenerCoordenadaDerSup();
	  // Si estaba a la derecha, y quiere terminar a la izquierda, se choca y no puede.
	if ((coordEste->x >= coordOtro->x) && (coordSig->x <= coordOtro->x) && (coordSig->y <= coordOtro->y)){
		coordSig->setearX(coordOtro->x);
		jugador->moverseAIzqSup(coordSig);
		delete coordSig;
		delete coordSig_fis;
		delete coordEste;
		delete coordOtro;
		return;
	}
	delete coordEste;
	delete coordOtro;
	
	// Caso: scrollear a la izquierda.
	if (this->ventana->coordenadaEnPantalla(coordSig_fis) == bordeIzq) {
	    scrollearIzquierda();
		float margen_izq = this->ventana->obtenerMargenLogicoIzquierdo(this->conversor);
		if (coordSig->x < margen_izq) coordSig->setearX(margen_izq);
		jugador->moverseAIzqSup(coordSig);
		delete coordSig;
		delete coordSig_fis;
		return;
	}
	    
	// Caso: la posición era válida en ancho.
	jugador->moverseAIzqSup(coordSig);
	delete coordSig;
	delete coordSig_fis;

}

/* Por ahora, sólo valida el movimiento de UN JUGADOR. */
void Director::verificar_movimientos(){
	///cout << "Jugador 1" << endl;
	verificar_movimiento(jugadores[jugador1], jugadores[jugador2]);
	///cout << "Jugador 2" << endl;
	verificar_movimiento(jugadores[jugador2], jugadores[jugador1]);
	
	// chequear colisiones:
	std::vector<Rectangulo*>* rectangulos_jug1 = jugadores[jugador1]->obtenerPersonaje()->accionActual->rectangulos;
	std::vector<Rectangulo*>* rectangulos_jug2 = jugadores[jugador2]->obtenerPersonaje()->accionActual->rectangulos;

	// esta verificación es porque todavíá todo no tiene rectangulos
	if (rectangulos_jug1->size() && rectangulos_jug2->size()){

		for (unsigned int i = 0; i < rectangulos_jug1->size(); i++){
			for (unsigned int j = 0; j < rectangulos_jug1->size(); j++){
				// verifico las colisiones
				SDL_Rect interseccion; // no lo usamos
				SDL_bool coli = SDL_IntersectRect(
					rectangulos_jug1->at(j)->sdl_rec,
					rectangulos_jug2->at(i)->sdl_rec, 
					&interseccion
				);
				
				if (coli){
					if (rectangulos_jug1->at(i)->ataque ^ rectangulos_jug2->at(j)->ataque){
						Jugador* sufre, *pegando;

						if (rectangulos_jug1->at(i)->ataque){
							pegando = jugadores[jugador1];
							sufre = jugadores[jugador2];
						} else {
							pegando = jugadores[jugador2];
							sufre = jugadores[jugador1];
						}

						sufre->barra->Lastimar(
							pegando->obtenerPersonaje()->accionActual->porcentajeDeDanio
						);
						this->escenario->Temblar(SDL_GetTicks());
						Logger::instance()->log_debug("Le pego!!!");

					}
				}

			}
		}

		
	}
}

bool Director::sePuedeScrollearDerecha(){
	float borde_der = this->ventana->obtenerBordeLogicoDerecho(this->conversor);
	// Si ya está al limite derecho del escenario, no se puede.
	bool sePuede = not (this->escenario->esLimiteDerecho(borde_der));
	// Si algun personaje quiere moverse más allá del borde izquierdo, no se puede scrollear a la derecha.
		//Jugador1
	CoordenadaLogica* coord = jugadores[jugador1]->obtenerSiguienteCoordenadaIzqSup();
	CoordenadaFisica* coord_fis = this->conversor->aFisica(coord);
	sePuede &= not (this->ventana->coordenadaEnPantalla(coord_fis) == bordeIzq);
	delete coord;
	delete coord_fis;
		//Jugador2
	coord = jugadores[jugador2]->obtenerSiguienteCoordenadaIzqSup();
	coord_fis = this->conversor->aFisica(coord);
	sePuede &= not (this->ventana->coordenadaEnPantalla(coord_fis) == bordeIzq);
	delete coord;
	delete coord_fis;
	return sePuede;
}

bool Director::sePuedeScrollearIzquierda(){
	float borde_izq = this->ventana->obtenerBordeLogicoIzquierdo(this->conversor);
	// Si ya está al limite izquierdo del escenario, no se puede.
	bool sePuede = not (this->escenario->esLimiteIzquierdo(borde_izq));
	// Si algun personaje quiere moverse más allá del borde derecho, no se puede scrollear para la izquierda.
		//Jugador1
	CoordenadaLogica* coord = jugadores[jugador1]->obtenerSiguienteCoordenadaDerSup();
	CoordenadaFisica* coord_fis = this->conversor->aFisica(coord);
	sePuede &= not (this->ventana->coordenadaEnPantalla(coord_fis) == bordeDer);
	delete coord;
	delete coord_fis;
		//Jugador2
	coord = jugadores[jugador2]->obtenerSiguienteCoordenadaDerSup();
	coord_fis = this->conversor->aFisica(coord);
	sePuede &= not (this->ventana->coordenadaEnPantalla(coord_fis) == bordeDer);
	delete coord;
	delete coord_fis;
	return sePuede;
}

void Director::scrollearDerecha(){
	if (not this->sePuedeScrollearDerecha()){
		return;
	}
	float borde_der = this->ventana->obtenerBordeLogicoDerecho(this->conversor);
	if (this->escenario->esLimiteDerecho(borde_der+factor_scroll)){
		this->conversor->seMueveVentana(this->escenario->obtenerLimiteDerecho() - borde_der);
	} else {
		this->conversor->seMueveVentana(factor_scroll);
	}
}

void Director::scrollearIzquierda(){
	if (not this->sePuedeScrollearIzquierda()) return;
	float borde_izq = this->ventana->obtenerBordeLogicoIzquierdo(this->conversor);
	if (this->escenario->esLimiteIzquierdo(borde_izq-factor_scroll))
		this->conversor->seMueveVentana(this->escenario->obtenerLimiteIzquierdo() - borde_izq);
	else this->conversor->seMueveVentana(- factor_scroll);
}



void Director::verificar_orientaciones(){
	//~ // Cuando haya dos jugadores, se descomenta.
	CoordenadaLogica* coord1 = jugadores[jugador1]->obtenerCoordenadaIzqSup();
	CoordenadaLogica* coord2 = jugadores[jugador2]->obtenerCoordenadaIzqSup();
	if (coord1->estaALaDerechaDe(coord2)){
		jugadores[jugador1]->mirarParaIzquierda();
		//~ this->informar_accion(MirarIzquierda,jugadores[jugador1]);
		if (!jugadores[jugador2]->personaje->mirarDerecha){
		
			puts("hola");
			jugadores[jugador2]->mirarParaDerecha();
		
			this->informar_accion(MirarDerecha,jugadores[jugador2]);
		}
	} else {
		jugadores[jugador1]->mirarParaDerecha();
		//~ this->informar_accion(MirarDerecha,jugadores[jugador1]);
		jugadores[jugador2]->mirarParaIzquierda();
		//~ this->informar_accion(MirarIzquierda,jugadores[jugador2]);
	}
	delete coord1;
	delete coord2;
}

/*********************************************************************
 * 
 * 						FUNCIONES PUBLICAS
 * 
 *********************************************************************/

/* Siempre va a pasar por acá antes del actualizar porque si no apretan
 * nada le va a mandar Nada (que sería parar). */
void Director::seMuevePersonaje(num_jugador jugador, movimiento lugar){
	switch (jugador){
		case jugador1:
			this->mov1 = lugar;
			break;
		default: //jugador2
			this->mov2 = lugar;
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
	//~ // VER BIEN SOBRE SPRITE DE CAMBIO DE ORIENTACIÓN !
	verificar_orientaciones();
}
