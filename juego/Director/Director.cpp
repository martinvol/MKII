#include <sstream>


#include "Director.hpp"
#include "../Logger/Logger.hpp"

/*********************************************************************
 * 
 * 					CONSTRUCTOR Y DESTRUCTOR
 * 
 *********************************************************************/

Director::Director(Escenario* escenario, Ventana* ventana, ConversorDeCoordenadas* conversor, float y_piso, Personaje* personaje1, Personaje* personaje2, BarraDeVida* barra1, BarraDeVida* barra2, Timer* timer){
	this->escenario = escenario;
	this->ventana = ventana;
	this->conversor = conversor;
	this->y_piso = y_piso;
	jugadores.push_back(new Jugador(personaje1, barra1));
	jugadores.push_back(new Jugador(personaje2, barra2));
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
 * 						  CLASES AUXILIARES
 * 
 *********************************************************************/

class Rect_Per {
public:
	CoordenadaLogica* izq_inf;
	CoordenadaLogica* der_inf;
	CoordenadaLogica* izq_sup;
	CoordenadaLogica* der_sup;
	float ancho;
	float alto;
	
	Rect_Per(CoordenadaLogica* izq_inf, float ancho, float alto){
		this->izq_inf = new CoordenadaLogica(izq_inf);
		this->der_inf = new CoordenadaLogica(izq_inf->x + ancho, izq_inf->y);
		this->izq_sup = new CoordenadaLogica(izq_inf->x, izq_inf->y + alto);
		this->der_sup = new CoordenadaLogica(izq_inf->x + ancho, izq_inf->y + alto);
		this->ancho = ancho;
		this->alto = alto;
	}
	
	~Rect_Per(){
		delete izq_inf;
		delete der_inf;
		delete izq_sup;
		delete der_sup;
	}
	
	void moverAIzqInf(float x, float y){
		delete this->izq_inf;
		delete this->der_inf;
		delete this->izq_sup;
		delete this->der_sup;
		this->izq_inf = new CoordenadaLogica(x, y);
		this->der_inf = new CoordenadaLogica(x + ancho, y);
		this->izq_sup = new CoordenadaLogica(x, y + alto);
		this->der_sup = new CoordenadaLogica(x + ancho, y + alto);
	}
	
	void moverADerInf(float x, float y){
		delete this->izq_inf;
		delete this->der_inf;
		delete this->izq_sup;
		delete this->der_sup;
		this->izq_inf = new CoordenadaLogica(x - ancho, y);
		this->der_inf = new CoordenadaLogica(x, y);
		this->izq_sup = new CoordenadaLogica(x - ancho, y + alto);
		this->der_sup = new CoordenadaLogica(x, y + alto);
	}
	
	void moverAIzqSup(float x, float y){
		delete this->izq_inf;
		delete this->der_inf;
		delete this->izq_sup;
		delete this->der_sup;
		this->izq_inf = new CoordenadaLogica(x, y - alto);
		this->der_inf = new CoordenadaLogica(x + ancho, y - alto);
		this->izq_sup = new CoordenadaLogica(x, y);
		this->der_sup = new CoordenadaLogica(x + ancho, y);
	}
	
	void moverADerSup(float x, float y){
		delete this->izq_inf;
		delete this->der_inf;
		delete this->izq_sup;
		delete this->der_sup;
		this->izq_inf = new CoordenadaLogica(x - ancho, y - alto);
		this->der_inf = new CoordenadaLogica(x, y - alto);
		this->izq_sup = new CoordenadaLogica(x - ancho, y);
		this->der_sup = new CoordenadaLogica(x, y);
	}
	
	bool estaALaDerechaDe(Rect_Per* otro){
		return (this->izq_inf->x >= otro->der_inf->x);
	}
	
	bool estaALaIzquierdaDe(Rect_Per* otro){
		return (this->der_inf->x <= otro->izq_inf->x);
	}
	
	bool estaEncimaDe(Rect_Per* otro){
		return (this->izq_inf->y >= otro->izq_sup->y);
	}

	bool estaDebajoDe(Rect_Per* otro){
		return (this->izq_sup->y <= otro->izq_inf->y);
	}
	
	bool estaSuperpuestoEnYCon(Rect_Per* otro){
		return ((this->izq_inf->y <= otro->izq_sup->y) && (this->izq_inf->y >= otro->izq_inf->y)
				|| (otro->izq_inf->y <= this->izq_sup->y) && (otro->izq_inf->y >= this->izq_inf->y));
	}
	
	bool estaSuperpuestoEnXCon(Rect_Per* otro){
		return ((this->izq_inf->x <= otro->der_inf->x) && (this->izq_inf->x >= otro->izq_inf->x)
				|| (otro->izq_inf->x <= this->der_inf->x) && (otro->izq_inf->x >= this->izq_inf->x));
	}

	bool estaAMismaAlturaQue(Rect_Per* otro){
		return ((this->izq_inf->y >= otro->izq_inf->y && this->izq_sup->y <= otro->izq_sup->y)
				||
				(otro->izq_inf->y >= this->izq_inf->y && otro->izq_sup->y <= this->izq_sup->y));
	}

	bool estaAMismoAnchoQue(Rect_Per* otro){
		return ((this->izq_inf->x >= otro->izq_inf->x && this->der_inf->x <= otro->der_inf->x)
				||
				(otro->izq_inf->x >= this->izq_inf->x && otro->der_inf->x <= this->der_inf->x));
	}

};

/*********************************************************************
 * 
 * 						AUXILIARES ṔRIVADAS
 * 
 *********************************************************************/

void Director::verificar_movimiento(Jugador* jugador, Jugador* elOtro){
	Rect_Per* rect_este = new Rect_Per(jugador->obtenerCoordenadaIzqInf(), jugador->personaje->ancho, jugador->personaje->alto);
	Rect_Per* rect_sig = new Rect_Per(jugador->obtenerSiguienteCoordenadaIzqInf(), jugador->personaje->ancho, jugador->personaje->alto);
	Rect_Per* rect_otro = new Rect_Per(elOtro->obtenerCoordenadaIzqInf(), elOtro->personaje->ancho, elOtro->personaje->alto);


	// Verifica altura.
	CoordenadaFisica* coordSig_fis = this->conversor->aFisica(rect_sig->izq_sup);
	if (this->ventana->superaTecho(coordSig_fis)){
		cout << "ENTRO TECHO" << endl;
		rect_sig->moverAIzqSup(rect_sig->izq_sup->x, this->ventana->obtenerBordeLogicoSuperior(this->conversor));
		
		jugador->personaje->estado->piniaAire->alcanzo_max = true; //%
		jugador->personaje->estado->patadaVert->alcanzo_max = true; //%
		jugador->personaje->estado->patadaDiag->alcanzo_max = true; //% 
		jugador->personaje->estado->piniaAireVertical->alcanzo_max = true; //%
	}
	delete coordSig_fis;
	
	// Verifica piso
	if (rect_sig->izq_inf->y < this->y_piso){
		cout << "ENTRO PISO" << endl;
		rect_sig->moverAIzqInf(rect_sig->izq_inf->x, this->y_piso);
	}
	
	// Verifica si se choca con el otro personaje.
		// Tienen que estar a la misma altura (el sig con el otro).
	
	// Choca por Lado Derecho de Este jugador:
		  // Si estaba a la izquierda, y quiere terminar a la derecha, se choca y no puede.
	cout << "Verificar choque por DERECHA" << endl;
	cout << "ESTE a la izquierda: " << rect_este->estaALaIzquierdaDe(rect_otro) << endl;
	cout << "borde derecho de ESTE: " << rect_este->der_inf->x << " y borde izquierdo de OTRO: " << rect_otro->izq_inf->x << endl;
	cout << "SIG a la derecha: " << rect_sig->estaALaDerechaDe(rect_otro) << endl;
	cout << "SIG superpuesto en X: " << rect_sig->estaSuperpuestoEnXCon(rect_otro) << endl;
	cout << "SIG y OTRO a misma altura: " << rect_sig->estaAMismaAlturaQue(rect_otro) << endl;
	if (rect_este->estaALaIzquierdaDe(rect_otro) && 
		(rect_sig->estaALaDerechaDe(rect_otro) || rect_sig->estaSuperpuestoEnXCon(rect_otro)) &&
		rect_sig->estaAMismaAlturaQue(rect_otro)){
		cout << "Entra CHOQUE DERECHA" << endl;
		rect_sig->moverADerInf(rect_otro->izq_inf->x, rect_sig->der_inf->y);
	}
	
	// Choca por Lado Izquierdo de Este jugador:
		// Si estaba a la derecha, y quiere terminar a la izquierda, se choca y no puede.
	if (rect_este->estaALaDerechaDe(rect_otro) && 
		(rect_sig->estaALaIzquierdaDe(rect_otro) || rect_sig->estaSuperpuestoEnXCon(rect_otro)) &&
		rect_sig->estaAMismaAlturaQue(rect_otro)){
		cout << "Entra CHOQUE IZQUIERDA" << endl;
		rect_sig->moverAIzqInf(rect_otro->der_inf->x, rect_sig->izq_inf->y);
	}
	
	// Cae encima del otro.
	if (rect_este->estaEncimaDe(rect_otro) &&
		(rect_sig->estaDebajoDe(rect_otro) || rect_sig->estaSuperpuestoEnYCon(rect_otro)) &&
		rect_sig->estaSuperpuestoEnXCon(rect_otro)){
		cout << "ENTRO CAER ENCIMA" << endl;
		// Si se mueve para la derecha
		if (rect_este->izq_inf->x <= rect_sig->izq_inf->x){
			// Intento ponerlo a la derecha del otro.
			// Si no se puede (scrollear), a la izquierda.
			CoordenadaLogica* sig_der = new CoordenadaLogica(rect_otro->der_inf->x + rect_sig->ancho, rect_sig->der_inf->y);
			CoordenadaFisica* sig_der_fis = this->conversor->aFisica(sig_der);
			if (this->ventana->coordenadaEnPantalla(sig_der_fis) == bordeDer && !sePuedeScrollearDerecha()){
				rect_sig->moverADerInf(rect_otro->izq_inf->x, rect_sig->der_inf->y);
			} else {
				rect_sig->moverAIzqInf(rect_otro->der_inf->x, rect_sig->izq_inf->y);
			}
			delete sig_der;
			delete sig_der_fis;
		}
		// Si se mueve para la izquierda
		else {
			// Intento ponerlo a la izquierda del otro.
			// Si no se puede (scrollear), a la derecha.
			CoordenadaLogica* sig_izq = new CoordenadaLogica(rect_otro->izq_inf->x - rect_sig->ancho, rect_sig->izq_inf->y);
			CoordenadaFisica* sig_izq_fis = this->conversor->aFisica(sig_izq);
			if (this->ventana->coordenadaEnPantalla(sig_izq_fis) == bordeIzq && !sePuedeScrollearIzquierda()){
				rect_sig->moverAIzqInf(rect_otro->der_inf->x, rect_sig->izq_inf->y);
			} else {
				rect_sig->moverADerInf(rect_otro->izq_inf->x, rect_sig->der_inf->y);
			}
			delete sig_izq;
			delete sig_izq_fis;
		}
	}
	
	// Scrollear a la derecha.
	CoordenadaFisica* coord_der = this->conversor->aFisica(rect_sig->der_inf);
	// (jugador->personaje->nroAccionActual != QUIETO) && 
	if ((this->ventana->coordenadaEnPantalla(coord_der) == bordeDer)){
		cout << "ENTRO SCROLL DERECHA" << endl;
		scrollearDerecha(rect_sig->der_inf->x - this->ventana->obtenerMargenLogicoDerecho(this->conversor));
		float margen_der = this->ventana->obtenerMargenLogicoDerecho(this->conversor);
		if (rect_sig->der_inf->x > margen_der){
			rect_sig->moverADerInf(margen_der, rect_sig->der_inf->y);
		}
	}
	delete coord_der;
	
	// Scrollear izquierda
	CoordenadaFisica* coord_izq = this->conversor->aFisica(rect_sig->izq_inf);
	// (jugador->personaje->nroAccionActual != QUIETO) && 
	if ((this->ventana->coordenadaEnPantalla(coord_izq) == bordeIzq)) {
		cout << "ENTRO SCROLL IZQUIERDA" << endl;
	    scrollearIzquierda(this->ventana->obtenerMargenLogicoIzquierdo(this->conversor) - rect_sig->izq_inf->x);
		float margen_izq = this->ventana->obtenerMargenLogicoIzquierdo(this->conversor);
		if (rect_sig->izq_inf->x < margen_izq){
			rect_sig->moverAIzqInf(margen_izq, rect_sig->izq_inf->y);
		}
	}
	delete coord_izq;
	
	jugador->moverseAIzqInf(rect_sig->izq_inf);
}

bool IntersectRect(const SDL_Rect * r1, const SDL_Rect * r2){
	if (r1== NULL || r2 == NULL){
		return false;
	}
	bool out = !((r2->x > (r1->x + r1->w)) || ((r2->x + r2->w) < r1->x) || (r2->y > (r1->y + r1->h)) || ((r2->y + r2->h) < r1->y));
	return out;
	// return false;
}

void Director::verificar_movimientos(){
	verificar_movimiento(jugadores[jugador1], jugadores[jugador2]);
	verificar_movimiento(jugadores[jugador2], jugadores[jugador1]);

	// chequear colisiones:
	std::vector<Rectangulo*>* rectangulos_jug1 = jugadores[jugador1]->obtenerPersonaje()->accionActual->rectangulos;
	std::vector<Rectangulo*>* rectangulos_jug2 = jugadores[jugador2]->obtenerPersonaje()->accionActual->rectangulos;

	// esta verificación es porque todavíá todo no tiene rectangulos
	if (rectangulos_jug1->size() && rectangulos_jug2->size()){

		for (unsigned int i = 0; i < rectangulos_jug1->size(); i++){
			for (unsigned int j = 0; j < rectangulos_jug2->size(); j++){
				// verifico las colisiones
				bool coli = IntersectRect(
					rectangulos_jug1->at(i)->sdl_rec,
					rectangulos_jug2->at(j)->sdl_rec
				);
				
				Rectangulo* recibe=NULL;
				if (coli && jugadores[jugador1]->obtenerPersonaje()->accionActual->dibuje_rectangulos && jugadores[jugador1]->obtenerPersonaje()->accionActual->dibuje_rectangulos){
					if (rectangulos_jug1->at(i)->ataque ^ rectangulos_jug2->at(j)->ataque){
						Jugador* sufre, *pegando;

						if (rectangulos_jug1->at(i)->ataque){
							pegando = jugadores[jugador1];
							sufre = jugadores[jugador2];
							recibe = rectangulos_jug2->at(j);
						} else {
							pegando = jugadores[jugador2];
							sufre = jugadores[jugador1];
							recibe = rectangulos_jug1->at(i);
						} 

						// Este if hace que solo se le pueda sacr vida una sola vez
						if (!pegando->obtenerPersonaje()->accionActual->saque_vida){

							float danio = pegando->obtenerPersonaje()->accionActual->porcentajeDeDanio;							
							if (recibe->bloqueo){
								Logger::instance()->log_debug("Le tengo que sacar menos vida porque se está defendiendo");	
								danio = danio/4.;
							}
							//Se lo lastima con un numero 0-1000
							sufre->barra->Lastimar(danio);
							// Esta linea fea hace la conversion numero -> string
							string result;ostringstream convert;convert << danio;result = convert.str(); 
							Logger::instance()->log_debug(string("Personaje recibe daño: ") +  result);
							//sufre->personaje->activarAccion(RECIBIRGOLPEALTO);

							if (!recibe->bloqueo){
								cout << pegando->personaje->accionActual->accionNro << endl;
								if(pegando->personaje->accionActual->accionNro == PATADAALTAAGACHADO){
									sufre->personaje->activarAccion(CAERPORTRABA);
								} else if (pegando->personaje->accionActual->accionNro == 21){
									sufre->personaje->activarAccion(CAERPORGANCHO);
								} else if (pegando->personaje->accionActual->accionNro == 7 ||
										   pegando->personaje->accionActual->accionNro == 34){
									sufre->personaje->activarAccion(RECIBIRGOLPEBAJO);
								} else if (sufre->personaje->accionActual->accionNro == 5){
									sufre->personaje->activarAccion(RECIBIRGOLPEAGACHADO); ///ESto aun no se puede probar
								}else {
									sufre->personaje->activarAccion(RECIBIRGOLPEALTO);
									cout<<sufre->personaje->accionActual->accionNro<<endl;
								}
							}
							this->escenario->Temblar(SDL_GetTicks());
							pegando->obtenerPersonaje()->accionActual->saque_vida = true; // Para que no le saque vida dos veces
						}

					}
				}
			}
		}
	}

	for (unsigned int i = 0; i<jugadores.size(); i++){
		if (jugadores[i]->obtenerPersonaje()->arrojable){
		
			if (jugadores[i]->obtenerPersonaje()->arrojable->salioDeLaPantalla(this->conversor)){
				Logger::instance()->log_debug("Borré el arma, salió de la pantalla o ya colisionó");
				delete jugadores[i]->obtenerPersonaje()->arrojable;
				jugadores[i]->obtenerPersonaje()->arrojable = NULL;
			}
		}
	}

	for (unsigned int i = 0; i<jugadores.size(); i++){
		if (jugadores[i]->obtenerPersonaje()->arrojable){
			for (unsigned int j = 0; j < jugadores[(i+1)%2]->obtenerPersonaje()->accionActual->rectangulos->size(); j++){
				// verifico las colisiones
				SDL_Rect interseccion; // no lo usamos
				SDL_bool coli = SDL_IntersectRect(
					jugadores[i]->obtenerPersonaje()->arrojable->rectangulo->sdl_rec,
					jugadores[(i+1)%2]->obtenerPersonaje()->accionActual->rectangulos->at(j)->sdl_rec, 
					&interseccion
				);
				
				if (coli){
					Logger::instance()->log_debug("Le pego el arrojable!!!");
					jugadores[i]->obtenerPersonaje()->arrojable->pego = true;
					float danio = 30;
					if (jugadores[(i+1)%2]->obtenerPersonaje()->accionActual->rectangulos->at(j)->bloqueo){
						Logger::instance()->log_debug("Le tengo que sacar menos vida porque se está defendiendo");
						danio = danio/4;
					}
					// Esta linea fea hace la conversion numero -> string
					string result;ostringstream convert;convert << danio;result = convert.str(); 
					Logger::instance()->log_debug(string("Personaje recibe daño: ") +  result);
					jugadores[(i+1)%2]->barra->Lastimar(danio);
					this->escenario->Temblar(SDL_GetTicks());
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

void Director::scrollearDerecha(float factor_scroll){
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

void Director::scrollearIzquierda(float factor_scroll){
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
		//~ jugadores[jugador1]->activarAccion(MIRARIZQUIERDA);
		jugadores[jugador2]->mirarParaDerecha();
		if (!jugadores[jugador2]->personaje->mirarDerecha){
		
			puts("hola");
			jugadores[jugador2]->activarAccion(MIRARDERECHA);
		}
	} else {
		jugadores[jugador1]->mirarParaDerecha();
		//~ jugadores[jugador1]->activarAccion(MIRARDERECHA);
		jugadores[jugador2]->mirarParaIzquierda();
		//~ jugadores[jugador2]->activarAccion(MIRARIZQUIERDA);
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

bool Director::seMurio(int num_jugador){
	return this->jugadores[num_jugador]->barra->seMurio();
}
