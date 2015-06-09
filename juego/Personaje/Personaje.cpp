#include <stdio.h>
#include <iostream>
#include "Personaje.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>


using namespace std;


/***********************************************************************
 * 
 * 							CONSTRUCTOR
 * 						   Y DESTRUCTOR
 *
 **********************************************************************/  

Personaje::Personaje(CoordenadaLogica* coord, string nombre,SDL_Renderer* ren, float alto, float ancho, Estado* estado, ConversorDeCoordenadas* conversor, float velocidad_arma, int numeroJugador, bool miraADerecha, std::vector<TomaData*>* tomas){
	this->tomas = tomas;

	this->numero_jugador = numeroJugador - 1;
	this->velocidad_arma = velocidad_arma;
	// 4 flechas
	Nada = Izquierda = Derecha = Arriba = Abajo = false;
	// 2 pinias
	PiniaAlta = PiniaBaja = false;
	// 2 patadas
	PatadaAlta = PatadaBaja = false;
	// arrojable y cubrirse
	ArrojarArma = CubrirAlto = false;
	
	this->alto = alto;
	this->ancho = ancho;
	this->conversor = conversor;
	this->y_inicial = coord->y;
	this->coordenada = coord;
	this->siguiente = new CoordenadaLogica(coord);
	this->mirarDerecha = miraADerecha;
	
	this->estado = estado;
	this->nroAccionActual = QUIETO;
	this->accionActual = this->estado->	quieto;
	this->imagenActual = this->accionActual->getImagenActual(mirarDerecha);
	
	calcularAnchoYAltoImagen();
	this->ancho_quieto = this->_w;
	this->altura_quieto = this->_h;

	CoordenadaLogica* coord1 = this->coordenada;
	CoordenadaFisica* coord1_fis = this->conversor->aFisica(coord1);
	CoordenadaLogica* coord2 = new CoordenadaLogica(coordenada->x + ancho, coordenada->y + alto);
	CoordenadaFisica* coord2_fis = this->conversor->aFisica(coord2);
	this->ancho_fisico = abs(coord1_fis->x_fisico - coord2_fis->x_fisico);		// Función de std
	this->alto_fisico = abs(coord1_fis->y_fisico - coord2_fis->y_fisico);
	delete coord2;
	delete coord1_fis;
	delete coord2_fis;
	
	this->nombrePersonaje = nombre;
	this->renderer = ren;

	// this->imagenArrojable = IMG_LoadTexture(this->renderer, this->estado->ruta_arrojable.c_str());;
	pinia_sonido = Mix_LoadWAV("resources/music/male_scream_short.wav");
}

Personaje::~Personaje(){
	delete panel;
	delete this->coordenada;
	delete this->estado;	// Esto elimina la acción y sus imágenes.
	delete this->siguiente;
	Mix_FreeChunk(this->pinia_sonido);
}

void Personaje::Arrojar(bool congelar){
	/// cout << "el personaje sabe que tiene que arrojar el arma" << endl; 
	if (this->arrojable == NULL){
		this->arrojable = new Arrojable(!this->mirarDerecha, this->renderer, congelar);
	
	
		CoordenadaLogica* coord1 = this->obtenerCoordenadaIzqInf();
		CoordenadaFisica* coord1_fis = this->conversor->aFisica(coord1);
		
		CoordenadaLogica* coord2 = this->obtenerCoordenadaDerSup();
		CoordenadaFisica* coord2_fis = this->conversor->aFisica(coord2);
		
		int ancho_fisico = abs(coord1_fis->x_fisico - coord2_fis->x_fisico);		// Función de std
		int alto_fisico = abs(coord1_fis->y_fisico - coord2_fis->y_fisico);
		CoordenadaLogica* desde;
		if (mirarDerecha){
			desde = this->obtenerCoordenadaIzqSup();
			arrojable->setCoordenadas(desde, alto_fisico, ancho_fisico);
		} else {
			desde = this->obtenerCoordenadaDerSup();
			arrojable->setCoordenadas(desde, alto_fisico, ancho_fisico);
		}

		arrojable->tirar(this->velocidad_arma);
	
		// desde la borra el personaje desde adentro
		delete coord1;
		delete coord1_fis;
		delete coord2;
		delete coord2_fis;
		
		this->cambiarAccionA(ARROJARARMA);
	}
}
/***********************************************************************
 * 
 * 					FUNCIONES PARA EL DIRECTOR
 *
 **********************************************************************/  

void Personaje::mirarParaDerecha(){
	mirarDerecha = true;
}

void Personaje::mirarParaIzquierda(){
	mirarDerecha = false;
}

void Personaje::activarAccion(accion_posible accion){	
	if (this->nroAccionActual != accion && (this->accionActual->permiteAccion(accion))){
		cambiarAccionA(accion);
	} else {
		delete siguiente;
		siguiente = this->accionActual->execute(this->coordenada);				
		switch (nroAccionActual){			
			case SALTAR:
				if (accion == PATADASALTANDOVERTICAL || accion == PATADAALTA || accion == PATADABAJA){
					cambiarAccionA(PATADASALTANDOVERTICAL);
				}
				else if (accion == PINIASALTANDOVERTICAL || accion == PINIABAJA || accion == PINIAALTA){	
					cambiarAccionA(PINIASALTANDOVERTICAL);	
				}
			case SALTARDIAGONAL_DER:
			case SALTARDIAGONAL_IZQ:
				if(accion == PATADASALTANDODIAGONAL || accion == PATADAALTA || accion == PATADABAJA){
					cambiarAccionA(PATADASALTANDODIAGONAL);
				}else if (accion == PINIASALTANDODIAGONAL || accion == PINIABAJA || accion == PINIAALTA){
					cambiarAccionA(PINIASALTANDODIAGONAL);
				}
			case CAERPORGANCHO:
			case PINIASALTANDODIAGONAL:
				if (siguiente->y < y_inicial) this->estado->piniaAire->alcanzo_max = false;
			case PINIASALTANDOVERTICAL:
			case PATADASALTANDODIAGONAL:
				if (siguiente->y < y_inicial) this->estado->patadaDiag->alcanzo_max = false;
			case PATADASALTANDOVERTICAL:
				if (siguiente->y < y_inicial){
					this->estado->saltardiagonal->alcanzo_max = false;
					this->estado->saltarvertical->alcanzo_max = false;
					this->estado->patadaVert->alcanzo_max = false;
					this->estado->piniaAireVertical->alcanzo_max = false;
					if (nroAccionActual == CAERPORGANCHO ){
						cambiarAccionA(LEVANTARSEDELGANCHO);
					}else{
						cambiarAccionA(QUIETO);
					}
					CoordenadaLogica* coord = new CoordenadaLogica(siguiente->x, y_inicial);
					delete siguiente;
					siguiente = coord;
				}
				break;			
			
			case GANCHO:
			case PATADAALTAAGACHADO:
			case PATADABAJAAGACHADO:	
			case PINIAAGACHADO:			
				if (this->accionActual->ciclos == 1){		
					cambiarAccionA(AGACHARSE);	
					this->accionActual->setModoActual(this->accionActual->cantModos-1);
				}				
				break;
			case CAERPORTRABA:
				if (this->accionActual->ciclos == 1){
					cambiarAccionA(LEVANTARSEDELATRABA);
				}
			break;
			case RECIBIRGOLPEALTO: ///		
			case RECIBIRGOLPEBAJO:
			case RECIBIRGOLPEAGACHADO:
			case PATADAALTA:
			case PATADABAJA:
			case ARROJARARMA:
			case PINIABAJA:
			//~ case PINIAAGACHADO:
			case PINIAALTA:			
			case TRABA:
			case ROUNDKICK:
			case LEVANTARSEDELGANCHO:
			case LEVANTARSEDELATRABA:				
				if (this->accionActual->ciclos == 1){
					cambiarAccionA(QUIETO);
				}
			break;			
			case GANAR:
			case MORIR:
			case DIZZY:				
				break;
			case PARARSE:				
				if(this->accionActual->modoActual == 0){
					cambiarAccionA(QUIETO);	
				}
			break;
			case AGACHARSE:	
				if(accion == PINIAAGACHADO){
					cambiarAccionA(PINIAAGACHADO);
				}
															
			case CUBRIRBAJO:
				break;	
			default:
				break;
		}
	}

	this->imagenActual = this->accionActual->getImagenActual(mirarDerecha);	
	
	//~ calcularAnchoYAltoImagen();
	//~ calcularDatosDibujables();
	
}

CoordenadaLogica* Personaje::obtenerCoordenadaIzqSup(){
	//~ CoordenadaFisica* coord_fis_izq_sup = new CoordenadaFisica(this->x, this->y);
	//~ CoordenadaLogica* coord = this->conversor->aLogica(coord_fis_izq_sup);
	//~ delete coord_fis_izq_sup;
	//~ return coord;
	CoordenadaLogica* coord = new CoordenadaLogica(coordenada);
	coord->desplazarY(alto);
	return coord;
}

CoordenadaLogica* Personaje::obtenerCoordenadaIzqInf(){
	//~ CoordenadaFisica* coord_fis_izq_sup = new CoordenadaFisica(this->x, this->y);
	//~ CoordenadaLogica* coord = this->conversor->aLogica(coord_fis_izq_sup);
	//~ coord->desplazarY(-_h);
	//~ delete coord_fis_izq_sup;
	//~ return coord;
	CoordenadaLogica* coord = new CoordenadaLogica(coordenada);
	return coord;
}

CoordenadaLogica* Personaje::obtenerCoordenadaDerSup(){
	//~ CoordenadaFisica* coord_fis_izq_sup = new CoordenadaFisica(this->x, this->y);
	//~ CoordenadaLogica* coord = this->conversor->aLogica(coord_fis_izq_sup);
	//~ coord->desplazarX(_w);
	//~ delete coord_fis_izq_sup;
	//~ return coord;
	CoordenadaLogica* coord = new CoordenadaLogica(coordenada);
	coord->desplazarY(alto);
	coord->desplazarX(ancho);
	return coord;
}

CoordenadaLogica* Personaje::obtenerCoordenadaDerInf(){
	//~ CoordenadaFisica* coord_fis_izq_sup = new CoordenadaFisica(this->x, this->y);
	//~ CoordenadaLogica* coord = this->conversor->aLogica(coord_fis_izq_sup);
	//~ coord->desplazarY(-_h);
	//~ coord->desplazarX(_w);
	//~ delete coord_fis_izq_sup;
	//~ return coord;
	CoordenadaLogica* coord = new CoordenadaLogica(coordenada);
	coord->desplazarX(ancho);
	return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaIzqSup(){
	CoordenadaLogica* coord = new CoordenadaLogica(siguiente);
	coord->desplazarY(alto);
	return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaIzqInf(){
	CoordenadaLogica* coord = new CoordenadaLogica(siguiente);
	return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaDerSup(){
	CoordenadaLogica* coord = new CoordenadaLogica(siguiente);
	coord->desplazarY(alto);
	coord->desplazarX(ancho);
	return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaDerInf(){
	CoordenadaLogica* coord = new CoordenadaLogica(siguiente);
	coord->desplazarX(ancho);
	return coord;
}

void Personaje::moverseAIzqSup(CoordenadaLogica* coord){
	delete coordenada;
	coordenada = new CoordenadaLogica(coord);
	coordenada->desplazarY(-alto);
	
}

void Personaje::moverseADerSup(CoordenadaLogica* coord){
	delete coordenada;
	coordenada = new CoordenadaLogica(coord);
	coordenada->desplazarY(-alto);
	coordenada->desplazarX(-ancho);
	
}

void Personaje::moverseAIzqInf(CoordenadaLogica* coord){
	delete coordenada;
	coordenada = new CoordenadaLogica(coord);
	
}

void Personaje::moverseADerInf(CoordenadaLogica* coord){
	delete coordenada;
	coordenada = new CoordenadaLogica(coord);
	coordenada->desplazarX(-ancho);
	
}

/***********************************************************************
 * 
 * 							DEMAS
 *
 **********************************************************************/  
/* Cuando se espera que el personaje represente una nueva accion, 
 * se destruye la Accion guardada
 * y se inicializa una nueva.
 * El tiempo se setea nuevamente en 0.
 * Recibe por parametro el numero que representa la nueva accion
 * un booleano que indica si la accion puede ser interrumpida.
 * y un puntero de tipo SDL_Renderer que indica el renderer.
 * */
void Personaje::cambiarAccionA(accion_posible nroAccion){
	
	this->accionActual->resetear();
	this->nroAccionActual = nroAccion;
	bool aux;
	bool llego_a_altura_max;
	this->accionActual->tinicial = SDL_GetTicks();
	
	switch (nroAccionActual)
	{ 
		case QUIETO:
			this->accionActual = this->estado->quieto;
			break;
		case CAMINAR_DERECHA:
			this->accionActual = this->estado->caminar;
			if (!this->mirarDerecha){
				this->accionActual->setInvertirSecuencia();
			}
			this->accionActual->setDireccionDerecha();
			break;
		case CAMINAR_IZQUIERDA:
			this->accionActual = this->estado->caminar;
			if(this->mirarDerecha){
				this->accionActual->setInvertirSecuencia();
			}
			this->accionActual->setDireccionIzquierda();
			break;
		case SALTAR:
			this->accionActual = this->estado->saltarvertical;
			break;
		case AGACHARSE:
			this->accionActual = this->estado->agacharse;			
			break;
		case PARARSE:
			this->accionActual = this->estado->agacharse;
			this->accionActual->setInvertirSecuencia();
			break;
		case SALTARDIAGONAL_DER:
			this->accionActual = this->estado->saltardiagonal;
			if(!this->mirarDerecha){
				this->accionActual->setInvertirSecuencia();
			}
			this->accionActual->setDireccionDerecha();
			break;	
		case PINIAALTA:
			gritar();
			this->accionActual = this->estado->piniaAlta;
			break;
		case PINIABAJA:
			gritar();
			this->accionActual = this->estado->piniaBaja;
			break;
		case PINIAAGACHADO:
			//cout<<"HOLA NRO:"<<PINIAAGACHADO<<endl;
			this->accionActual = this->estado->piniaAgachado;
			break;
		case PATADAALTA:
			this->accionActual = this->estado->patadaAlta;
			break;
		case PATADABAJA:
			this->accionActual = this->estado->patadaBaja;
			break;
		case ARROJARARMA:
			this->accionActual = this->estado->arrojarproyectil;
			break;
		case PATADABAJAAGACHADO:
			this->accionActual = this->estado->patadaBajaAgachado;
			break;
		case PATADAALTAAGACHADO:
			this->accionActual = this->estado->patadaAltaAgachado;
			break;
		case MIRARDERECHA:
			this->accionActual = this->estado->girar;
			if (!this->mirarDerecha){
				this->accionActual->setInvertirSecuencia();
			}
			this->accionActual->setDireccionDerecha();
			break;
		case MIRARIZQUIERDA:
			this->accionActual = this->estado->girar;
			if(this->mirarDerecha){
				this->accionActual->setInvertirSecuencia();
			}
			this->accionActual->setDireccionIzquierda();
			break;
		case CUBRIRALTO:
			this->accionActual = this->estado->cubrirAlto;
			break;
		case CUBRIRBAJO:
			this->accionActual = this->estado->cubrirBajo;
			break;
		case TRABA:			
			this->accionActual = this->estado->traba;
			break;
		case GANCHO:
			//cout<<"gancho"<<endl; ///
			this->accionActual = this->estado->gancho;
			break;
		case PINIASALTANDODIAGONAL:
			llego_a_altura_max = this->estado->saltardiagonal->alcanzo_max;
			aux = this->accionActual->direccionDerecha;
			this->accionActual = this->estado->piniaAire;
			this->estado->piniaAire->alcanzo_max = llego_a_altura_max;
			aux? this->accionActual->setDireccionDerecha():this->accionActual->setDireccionIzquierda();
			break;
		case PATADASALTANDODIAGONAL:
			llego_a_altura_max = this->estado->saltardiagonal->alcanzo_max;
			aux = this->accionActual->direccionDerecha;
			this->accionActual = this->estado->patadaDiag;
			this->estado->patadaDiag->alcanzo_max = llego_a_altura_max;
			aux? this->accionActual->setDireccionDerecha():this->accionActual->setDireccionIzquierda();
			break;
		case PATADASALTANDOVERTICAL:
			llego_a_altura_max = this->estado->saltarvertical->alcanzo_max;
			this->accionActual = this->estado->patadaVert;
			this->estado->patadaVert->alcanzo_max = llego_a_altura_max;
			break;
		case PINIASALTANDOVERTICAL:
			llego_a_altura_max = this->estado->saltarvertical->alcanzo_max;
			this->accionActual = this->estado->piniaAireVertical;
			this->estado->piniaAireVertical->alcanzo_max = llego_a_altura_max;
			break;
		case ROUNDKICK:
			this->accionActual = this->estado->roundKick;
			break;
		case RECIBIRGOLPEALTO:
			this->accionActual = this->estado->recibirgolpealto;
			break;
		case RECIBIRGOLPEBAJO:
			this->accionActual = this->estado->recibirgolpebajo;
			break;
		case CAERPORGANCHO:
			this->accionActual = this->estado->siendoarrojado;
			if(this->mirarDerecha){
				this->accionActual->setInvertirSecuencia();
				this->accionActual->setDireccionIzquierda();
			}
			break;
		case RECIBIRGOLPEAGACHADO:
			this->accionActual = this->estado->recibirGolpeAgachado;
			break;
		case CAERPORTRABA:
			this->accionActual = this->estado->recibirTraba;
			break;
		case LEVANTARSEDELGANCHO:
			this->accionActual = this->estado->levantarse;
			break;
		case LEVANTARSEDELATRABA:
			this->accionActual = this->estado->levantarseDelGancho;
			break;
		case GANAR:
			this->accionActual = this->estado->ganar;
			break;
		case MORIR:
			this->accionActual = this->estado->morir;
			break;
		case DIZZY:
			this->accionActual = this->estado->dizzy;
			break;
		default: // case SALTARDIAGONAL_IZQ:
			this->accionActual = this->estado->saltardiagonal;
			if(this->mirarDerecha){
				this->accionActual->setInvertirSecuencia();
			}
			this->accionActual->setDireccionIzquierda();
			break;
	}
		this->accionActual->saque_vida = false;
		this->accionActual->dibuje_rectangulos = false;	
	
	//~ calcularAnchoYAltoImagen();
}

float Personaje::calcularAnchoYAltoImagen(){
	SDL_QueryTexture(this->imagenActual, NULL, NULL, &(this->_w), &(this->_h));
}

void Personaje::calcularDatosDibujables(){
	CoordenadaLogica* coord1 = new CoordenadaLogica(this->coordenada);
	CoordenadaFisica* coord1_fis = this->conversor->aFisica(coord1);
	CoordenadaLogica* coord2 = new CoordenadaLogica(coordenada->x + ancho, coordenada->y + alto);
	CoordenadaFisica* coord2_fis = this->conversor->aFisica(coord2);

	
	this->x = coord1_fis->x_fisico;
	if (!this->mirarDerecha && nroAccionActual != QUIETO)
		this->x = coord1_fis->x_fisico + float(this->ancho_quieto - this->_w)*this->conversor->factor_ancho;
	this->y = coord2_fis->y_fisico + float(this->altura_quieto - this->_h)*this->conversor->factor_alto;
	this->w = (float(this->_w)/this->ancho_quieto)*ancho_fisico;
	this->h = (float(this->_h) / this->altura_quieto)*alto_fisico;

	delete coord1;
	delete coord2;
	delete coord1_fis;
	delete coord2_fis;
}

void Personaje::Dibujarse(){
	
	// Ancho y alto fisico y ancho y alto de quieto, en el constructor.

	calcularAnchoYAltoImagen();
	calcularDatosDibujables();

	//Rectangulo destino
	SDL_Rect destino;
	destino.x = this->x;
	destino.y = this->y;
	destino.h = this->h;
	destino.w = this->w;
	
	SDL_Point point = {this->_w/2, this->_h};
	
	
	//Se dibuja comun, this->imagenActual ya esta espejada, o no. 	
	SDL_RenderCopy(this->renderer, this->imagenActual, NULL, &destino);
	


	for(int i = 0; i < this->accionActual->rectangulos->size(); i++) {
		// Para evitar hacer esto acá podría crear un objeto
		// pero no quiero hacer una clase solo para este loop
		this->accionActual->rectangulos->at(i)->generar_rectanguloSDL(destino.x, destino.y, destino.w, destino.h ,renderer, !mirarDerecha);
			
	}
	this->accionActual->dibuje_rectangulos = true;
	
	if (this->arrojable){

		//SDL_RenderCopyEx(this->renderer, this->imagenActual, NULL, &destino,0,&point,SDL_FLIP_NONE);

		this->arrojable->dibujar(this->conversor);
	}
	
	/*puts(tomas->at(0)->nombre.c_str());
	puts(tomas->at(1)->nombre.c_str());*/
	
	if (panel){
		panel->dibujar(this->conversor, this->renderer);
		if (tomas->at(0)){
			if (panel->checkToma(tomas->at(0)->convinacion, tomas->at(0)->nombre)){
				// acá se activan las tomas
			}
		}

		if (tomas->at(1)){
			if (panel->checkToma(tomas->at(1)->convinacion, tomas->at(1)->nombre)){
				// acá se activan las tomas
				puts("tiro");
				this->Arrojar(false);
			}
		}
		if (tomas->at(2)){
			if (panel->checkToma(tomas->at(2)->convinacion, tomas->at(2)->nombre)){
				// acá se activan las tomas
				puts("tiro congelando");
				this->Arrojar(true);
			}
		}
	}
}

void Personaje::dibujar_botones(Parser* conf, bool debo_dibujar){
	puts("botones");
	panel = new PanelBotones(conf, renderer, this->numero_jugador, debo_dibujar);
	// acá se dibujan en pantala las cosas


}
/***********************************************************************
 * 
 * 							CONTROLADOR
 *
 **********************************************************************/  
 void Personaje::ActualizarControlador(SDL_Joystick *joystick, Parser* conf, SDL_Event *evento){

	SDL_JoystickID numeroJoystick = (evento->jdevice.which);		
	unordered_map <string, int>* conf_joys = conf->joysticks->at(this->numero_jugador);
	Uint8 i = evento->jbutton.button;
	
	switch (evento->type){
		case SDL_JOYAXISMOTION:
		if (numeroJoystick != (this->numero_jugador))
				break;
				
			if( evento->jaxis.axis == 0){
				/* Left-right movement code goes here */
				int x_Joystick = evento->jaxis.value;
				if( x_Joystick < -JOYSTICK_DEAD_ZONE ){		//  x = -1;		
					Izquierda = true;		
				}else if( x_Joystick > JOYSTICK_DEAD_ZONE ){//  x =  1;		
					Derecha = true;			
				}else{	//  x = 0;				
					Izquierda = false;
					Derecha = false;
				}
            
			}
			if( evento->jaxis.axis == 1){
				int y_Joystick = evento->jaxis.value;
            /* Up-Down movement code goes here */
				if( y_Joystick < -JOYSTICK_DEAD_ZONE ){ //  y = -1;		
					Arriba = true;		
				}else if( y_Joystick > JOYSTICK_DEAD_ZONE ){ //y =  1;		
					Abajo = true;		
				}else{ //yDir = 0;		
					Arriba = false;
					Abajo = false;
				}	
            
			}
			break;
		case SDL_JOYBUTTONDOWN:
			//Si el ID del joystick no corresponde
			//con el numero de jugador, no procese el evento.
			if (numeroJoystick != (this->numero_jugador))
				break;
				
			
			if (i ==  (*conf_joys)["pinia_baja"]){
				PiniaBaja = true;
			} else if (i == (*conf_joys)["cubrirse"]){
				CubrirAlto = true;
			} else if (i == (*conf_joys)["patada_baja"]){
				PatadaBaja = true;
			} else if (i == (*conf_joys)["pinia_alta"]){
				PiniaAlta = true;
			} else if (i == (*conf_joys)["arrojar_arma"]){
				//ArrojarArma = true;
				this->Arrojar(false);
			} else if (i == (*conf_joys)["arrojar_arma_baja"]){
				//ArrojarArma = true;
				this->Arrojar(false);
				if (this->arrojable != NULL){
					this->arrojable->tirarDiagonal(TIRAR_ARRIBA);
				}
			} else if (i == (*conf_joys)["arrojar_arma_alta"]){
				//ArrojarArma = true;
				this->Arrojar(false);
				if (this->arrojable != NULL){
					this->arrojable->tirarDiagonal(TIRAR_ABAJO);
				}
			} else if (i == (*conf_joys)["patada_alta"]){
				PatadaAlta = true;

			}
			if (panel){
				panel->AgregarBotones(i);
			}
			break;
		case SDL_JOYBUTTONUP:
		//Si el ID del joystick no corresponde
		//con el numero de jugador, no procese el evento.
			if (numeroJoystick != (this->numero_jugador))
				break;		
			
			if (i ==  (*conf_joys)["pinia_baja"]){
				PiniaBaja = false;					
			} else if (i == (*conf_joys)["cubrirse"]){
				CubrirAlto = false;
				if (Abajo == true){
					cambiarAccionA(AGACHARSE);	
					this->accionActual->setModoActual(this->accionActual->cantModos-1);					
				}
				CubrirAlto = false;
			} else if (i == (*conf_joys)["patada_baja"]){
				PatadaBaja = false;
			} else if (i == (*conf_joys)["pinia_alta"]){
				PiniaAlta = false;
			} else if (i == (*conf_joys)["arrojar_arma"]){				
				this->Arrojar(false);
			} else if (i == (*conf_joys)["patada_alta"]){
				PatadaAlta = false;
			}
			break;
	
	}
	 
}

void Personaje::gritar(){
	Mix_PlayChannel(-1, pinia_sonido, 0);
}
