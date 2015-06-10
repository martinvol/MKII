
#include "DirectorPractica.hpp"


#include <sstream>


#include "DirectorPractica.hpp"
#include "../Logger/Logger.hpp"

/*********************************************************************
 * 
 * 					CONSTRUCTOR Y DESTRUCTOR
 * 
 *********************************************************************/

DirectorPractica::DirectorPractica(Escenario* escenario, Ventana* ventana, ConversorDeCoordenadas* conversor, float y_piso, Personaje* personaje1, Personaje* personaje2, BarraDeVida* barra1, BarraDeVida* barra2, Timer* timer):Director(escenario, ventana, conversor, y_piso, personaje1, personaje2, barra1, barra2,timer){
	this->escenario = escenario;
	this->ventana = ventana;
	this->conversor = conversor;
	this->y_piso = y_piso;
	jugadores.push_back(new Jugador(personaje1, NULL));
	jugadores.push_back(new Jugador(personaje2, NULL)); // el NULL es la barra de vida/
	this->timer = NULL;
}

/*DirectorPractica::~DirectorPractica(){
	delete escenario;
	delete conversor;
	for (unsigned int i = 0; i <= jugadores.size() -1; i++){
        delete jugadores[i];
    }
    jugadores.clear();
}
*/

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
		return (((this->izq_inf->y <= otro->izq_sup->y) && (this->izq_inf->y >= otro->izq_inf->y))
				|| ((otro->izq_inf->y <= this->izq_sup->y) && (otro->izq_inf->y >= this->izq_inf->y)));
	}
	
	bool estaSuperpuestoEnXCon(Rect_Per* otro){
		return (((this->izq_inf->x <= otro->der_inf->x) && (this->izq_inf->x >= otro->izq_inf->x))
				|| ((otro->izq_inf->x <= this->der_inf->x) && (otro->izq_inf->x >= this->izq_inf->x)));
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

void DirectorPractica::verificar_movimiento(Jugador* jugador, Jugador* elOtro){
	CoordenadaLogica* temp1=jugador->obtenerCoordenadaIzqInf();
	CoordenadaLogica* temp2 = jugador->obtenerSiguienteCoordenadaIzqInf();
	CoordenadaLogica* temp3=elOtro->obtenerCoordenadaIzqInf();
	Rect_Per* rect_este = new Rect_Per(temp1, jugador->personaje->ancho, jugador->personaje->alto);
	Rect_Per* rect_sig = new Rect_Per(temp2, jugador->personaje->ancho, jugador->personaje->alto);
	Rect_Per* rect_otro = new Rect_Per(temp3, elOtro->personaje->ancho, elOtro->personaje->alto);
	delete temp1;
	delete temp2;
	delete temp3;

	// Verifica altura.
	CoordenadaFisica* coordSig_fis = this->conversor->aFisica(rect_sig->izq_sup);
	if (this->ventana->superaTecho(coordSig_fis)){
		rect_sig->moverAIzqSup(rect_sig->izq_sup->x, this->ventana->obtenerBordeLogicoSuperior(this->conversor));
		
		jugador->personaje->estado->piniaAire->alcanzo_max = true; //%
		jugador->personaje->estado->patadaVert->alcanzo_max = true; //%
		jugador->personaje->estado->patadaDiag->alcanzo_max = true; //% 
		jugador->personaje->estado->piniaAireVertical->alcanzo_max = true; //%
	}
	delete coordSig_fis;
	
	// Verifica piso
	if (rect_sig->izq_inf->y < this->y_piso){
		rect_sig->moverAIzqInf(rect_sig->izq_inf->x, this->y_piso);
	}
	
	// Verifica si se choca con el otro personaje.
		// Tienen que estar a la misma altura (el sig con el otro).
	bool choco = false;
	
	// Choca por Lado Derecho de Este jugador:
		  // Si estaba a la izquierda, y quiere terminar a la derecha, se choca y no puede.
	if (rect_este->estaALaIzquierdaDe(rect_otro) && 
		(rect_sig->estaALaDerechaDe(rect_otro) || rect_sig->estaSuperpuestoEnXCon(rect_otro)) &&
		(rect_sig->estaAMismaAlturaQue(rect_otro) || // Están a misma altura o
													  // Se superponen en ambos ejes y está bajando.
			(rect_sig->estaSuperpuestoEnXCon(rect_otro) && rect_sig->estaSuperpuestoEnYCon(rect_otro) && rect_sig->izq_inf->y <= rect_este->izq_inf->y))){
		rect_sig->moverADerInf(rect_otro->izq_inf->x, rect_sig->der_inf->y);
		choco = true;
	}
	
	// Choca por Lado Izquierdo de Este jugador:
		// Si estaba a la derecha, y quiere terminar a la izquierda, se choca y no puede.
	if (rect_este->estaALaDerechaDe(rect_otro) && 
		(rect_sig->estaALaIzquierdaDe(rect_otro) || rect_sig->estaSuperpuestoEnXCon(rect_otro)) &&
		(rect_sig->estaAMismaAlturaQue(rect_otro) || // Están a misma altura o
													  // Se superponen en ambos ejes y está bajando.
			(rect_sig->estaSuperpuestoEnXCon(rect_otro) && rect_sig->estaSuperpuestoEnYCon(rect_otro) && rect_sig->izq_inf->y <= rect_este->izq_inf->y))){
		rect_sig->moverAIzqInf(rect_otro->der_inf->x, rect_sig->izq_inf->y);
		choco = true;
	}
	
	// Cae encima del otro.
	float mov;
	if (!choco && rect_este->estaEncimaDe(rect_otro) &&
		(rect_sig->estaDebajoDe(rect_otro) || rect_sig->estaSuperpuestoEnYCon(rect_otro)) &&
		rect_sig->estaSuperpuestoEnXCon(rect_otro)){
		// Si se mueve para la derecha
		if (rect_este->izq_inf->x <= rect_sig->izq_inf->x){
			mov = rect_sig->izq_inf->x - rect_este->izq_inf->x;
			// Intento ponerlo a la derecha del otro.
			// Si no se puede (scrollear), a la izquierda.
			CoordenadaLogica* sig_der = new CoordenadaLogica(rect_otro->der_inf->x + rect_sig->ancho, rect_sig->der_inf->y);
			CoordenadaFisica* sig_der_fis = this->conversor->aFisica(sig_der);
			if (this->ventana->coordenadaEnPantalla(sig_der_fis) == bordeDer && !sePuedeScrollearDerecha()){
				rect_sig->moverADerInf(rect_otro->izq_inf->x, rect_sig->der_inf->y); //VOLPE
			} else {
				rect_sig->moverAIzqInf(rect_este->izq_inf->x + mov, rect_este->izq_inf->y); //VOLPE
			}
			delete sig_der;
			delete sig_der_fis;
		}
		// Si se mueve para la izquierda
		else {
			mov = rect_este->izq_inf->x - rect_sig->izq_inf->x;
			// Intento ponerlo a la izquierda del otro.
			// Si no se puede (scrollear), a la derecha.
			CoordenadaLogica* sig_izq = new CoordenadaLogica(rect_otro->izq_inf->x - rect_sig->ancho, rect_sig->izq_inf->y);
			CoordenadaFisica* sig_izq_fis = this->conversor->aFisica(sig_izq);
			if (this->ventana->coordenadaEnPantalla(sig_izq_fis) == bordeIzq && !sePuedeScrollearIzquierda()){
				rect_sig->moverAIzqInf(rect_otro->der_inf->x, rect_sig->izq_inf->y); //VOLPE
			} else {
				rect_sig->moverADerInf(rect_este->der_inf->x - mov, rect_sig->der_inf->y); //VOLPE
			}
			delete sig_izq;
			delete sig_izq_fis;
		}
	}
	
	// Scrollear a la derecha.
	CoordenadaFisica* coord_der = this->conversor->aFisica(rect_sig->der_inf);
	// (jugador->personaje->nroAccionActual != QUIETO) && 
	if ((this->ventana->coordenadaEnPantalla(coord_der) == bordeDer)){
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
	    scrollearIzquierda(this->ventana->obtenerMargenLogicoIzquierdo(this->conversor) - rect_sig->izq_inf->x);
		float margen_izq = this->ventana->obtenerMargenLogicoIzquierdo(this->conversor);
		if (rect_sig->izq_inf->x < margen_izq){
			rect_sig->moverAIzqInf(margen_izq, rect_sig->izq_inf->y);
		}
	}
	delete coord_izq;
	
	jugador->moverseAIzqInf(rect_sig->izq_inf);
	delete rect_sig;
	delete rect_este;
	delete rect_otro;

}

/*bool IntersectRect(const SDL_Rect * r1, const SDL_Rect * r2){
	if (r1== NULL || r2 == NULL){
		return false;
	}
	bool out = !((r2->x > (r1->x + r1->w)) || ((r2->x + r2->w) < r1->x) || (r2->y > (r1->y + r1->h)) || ((r2->y + r2->h) < r1->y));
	return out;
	// return false;
}
void presentarAnimacionRecibirGolpe(Personaje* pegando, Personaje* sufre) {
	
	if(pegando->accionActual->accionNro == PATADAALTAAGACHADO){
		sufre->activarAccion(CAERPORTRABA);
		return;
	}
	
	if(sufre->nroAccionActual == AGACHARSE || sufre->nroAccionActual == PATADAALTAAGACHADO ||
	sufre->nroAccionActual == PATADABAJAAGACHADO || sufre->nroAccionActual == PINIAAGACHADO) {
		sufre->activarAccion(RECIBIRGOLPEAGACHADO); 
		return;
	}
	
	//if (pegando->accionActual->accionNro == 21 || pegando->accionActual->accionNro == 25) {
    if (pegando->accionActual->accionNro == 21){
	    sufre->activarAccion(CAERPORGANCHO);
	} else if (pegando->accionActual->accionNro == 7 ||
	    pegando->accionActual->accionNro == 34 || pegando->nroAccionActual == PATADABAJA || 
	    pegando->nroAccionActual == PATADAALTAAGACHADO || pegando->nroAccionActual == PATADABAJAAGACHADO){
	    sufre->activarAccion(RECIBIRGOLPEBAJO);
	} else if (sufre->accionActual->accionNro == 5){
	    sufre->activarAccion(RECIBIRGOLPEAGACHADO); ///ESto aun no se puede probar
	}else sufre->activarAccion(RECIBIRGOLPEALTO);
	// cout<<sufre->personaje->accionActual->accionNro<<endl;								}
}
*/



void DirectorPractica::verificar_movimientos(){
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
							// sufre->barra->Lastimar(danio);
							// Esta linea fea hace la conversion numero -> string
							string result;ostringstream convert;convert << danio;result = convert.str(); 
							Logger::instance()->log_debug(string("Personaje recibe daño: ") +  result);
							//sufre->personaje->activarAccion(RECIBIRGOLPEALTO);

							if (!recibe->bloqueo){
								/// cout << pegando->personaje->accionActual->accionNro << endl;
								///cout << sufre->personaje->accionActual->accionNro<< endl;
								presentarAnimacionRecibirGolpe(pegando->obtenerPersonaje(), sufre->obtenerPersonaje());
								this->escenario->Temblar(SDL_GetTicks());
							}
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
				
				if (coli && jugadores[jugador1]->obtenerPersonaje()->accionActual->dibuje_rectangulos && jugadores[jugador1]->obtenerPersonaje()->accionActual->dibuje_rectangulos){
					Logger::instance()->log_debug("Le pego el arrojable!!!");
					jugadores[i]->obtenerPersonaje()->arrojable->pego = true;

					if (!jugadores[i]->obtenerPersonaje()->arrojable->congelar){
						float danio = 100;
						if (jugadores[(i+1)%2]->obtenerPersonaje()->accionActual->rectangulos->at(j)->bloqueo){
							Logger::instance()->log_debug("Le tengo que sacar menos vida porque se está defendiendo");
							danio = danio/4;
						}
						// Esta linea fea hace la conversion numero -> string
						string result;ostringstream convert;convert << danio;result = convert.str(); 
						Logger::instance()->log_debug(string("Personaje recibe daño: ") +  result);
						/// jugadores[(i+1)%2]->barra->Lastimar(danio);
						this->escenario->Temblar(SDL_GetTicks());
					} else {
						jugadores[(i+1)%2]->obtenerPersonaje()->congelarse();
					}
				}
			}
			
		}
	}

	for (unsigned int i = 0; i<jugadores.size(); i++){
		if (jugadores[i]->obtenerPersonaje()->latigo->coord){
			for (unsigned int j = 0; j < jugadores[(i+1)%2]->obtenerPersonaje()->accionActual->rectangulos->size(); j++){
				// verifico las colisiones
				SDL_Rect interseccion; // no lo usamos
				SDL_bool coli = SDL_IntersectRect(
					jugadores[i]->obtenerPersonaje()->latigo->rectangulo->sdl_rec,
					jugadores[(i+1)%2]->obtenerPersonaje()->accionActual->rectangulos->at(j)->sdl_rec, 
					&interseccion
				);
				
				if (coli && jugadores[jugador1]->obtenerPersonaje()->accionActual->dibuje_rectangulos && jugadores[jugador1]->obtenerPersonaje()->accionActual->dibuje_rectangulos){
					Logger::instance()->log_debug("Le pego el lático!!!");
					jugadores[i]->obtenerPersonaje()->latigo->volver = true;
					jugadores[i]->obtenerPersonaje()->latigo->pego = true;
					float danio = 100;
					if (jugadores[(i+1)%2]->obtenerPersonaje()->accionActual->rectangulos->at(j)->bloqueo){
						Logger::instance()->log_debug("Le tengo que sacar menos vida porque se está defendiendo");
						danio = danio/4;
					}
					// Esta linea fea hace la conversion numero -> string
					string result;ostringstream convert;convert << danio;result = convert.str(); 
					Logger::instance()->log_debug(string("Personaje recibe daño: ") +  result);
					/// jugadores[(i+1)%2]->barra->Lastimar(danio);
					CoordenadaLogica* mover =  jugadores[(i+1)%2]->obtenerSiguienteCoordenadaIzqInf();
					mover->x -=10;
					jugadores[(i+1)%2]->moverseAIzqInf(mover);
					delete mover;
				}
			}
		}
	}
}

bool DirectorPractica::sePuedeScrollearDerecha(){
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

bool DirectorPractica::sePuedeScrollearIzquierda(){
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

void DirectorPractica::scrollearDerecha(float factor_scroll){
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

void DirectorPractica::scrollearIzquierda(float factor_scroll){
	if (not this->sePuedeScrollearIzquierda()) return;
	float borde_izq = this->ventana->obtenerBordeLogicoIzquierdo(this->conversor);
	if (this->escenario->esLimiteIzquierdo(borde_izq-factor_scroll))
		this->conversor->seMueveVentana(this->escenario->obtenerLimiteIzquierdo() - borde_izq);
	else this->conversor->seMueveVentana(- factor_scroll);
}

void DirectorPractica::verificar_orientaciones(){
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
void DirectorPractica::seMuevePersonaje(num_jugador jugador, movimiento lugar){
	switch (jugador){
		case jugador1:
			this->mov1 = lugar;
			break;
		default: //jugador2
			this->mov2 = lugar;
			break;
	}
}

void DirectorPractica::actualizar(){
	
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

bool DirectorPractica::seMurio(int num_jugador){
	return false;
}
