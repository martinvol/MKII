#include <stdio.h>
#include <iostream>
#include "Personaje.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>


using namespace std;


/***********************************************************************
 * 
 * 							CONSTRUCTOR
 * 						   Y DESTRUCTOR
 *
 **********************************************************************/  

Personaje::Personaje(CoordenadaLogica* coord, string nombre,SDL_Renderer* ren, float alto, float ancho, Estado* estado, ConversorDeCoordenadas* conversor, float velocidad_arma, int numeroJugador, bool miraADerecha){

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
	this->imagenActual = this->accionActual->getImagenActual();
	
	this->nombrePersonaje = nombre;
	this->renderer = ren;

	this->imagenArrojable = IMG_LoadTexture(this->renderer, this->estado->ruta_arrojable.c_str());;
	this->altura_quieto = 0;
	this->ancho_quieto = 0;
}

Personaje::~Personaje(){
	delete this->coordenada;
	delete this->estado;	// Esto elimina la acción y sus imágenes.
}

void Personaje::Arrojar(){
	/// cout << "el personaje sabe que tiene que arrojar el arma" << endl; 

	if (this->arrojable == NULL){
		this->arrojable = new Arrojable(this->imagenArrojable, !this->mirarDerecha, this->renderer);
	
	
		CoordenadaLogica* coord1 = this->obtenerCoordenadaIzqInf();
		CoordenadaFisica* coord1_fis = this->conversor->aFisica(coord1);
		
		CoordenadaLogica* coord2 = this->obtenerCoordenadaDerSup();
		CoordenadaFisica* coord2_fis = this->conversor->aFisica(coord2);
		
		int ancho_fisico = abs(coord1_fis->x_fisico - coord2_fis->x_fisico);		// Función de std
		int alto_fisico = abs(coord1_fis->y_fisico - coord2_fis->y_fisico);
		if (mirarDerecha){
			arrojable->setCoordenadas(this->obtenerCoordenadaIzqSup(), alto_fisico, ancho_fisico);
		} else {
			arrojable->setCoordenadas(this->obtenerCoordenadaDerSup(), alto_fisico, ancho_fisico);
		}
		arrojable->tirar(this->velocidad_arma);
	
		delete coord1,coord1_fis,coord2,coord2_fis;
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
				if (accion == PATADASALTANDOVERTICAL){
					puts("holafijslkgjflkj");
					cambiarAccionA(PATADASALTANDOVERTICAL);
				}
			case SALTARDIAGONAL_DER:
			case SALTARDIAGONAL_IZQ:
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
					cambiarAccionA(QUIETO);
					CoordenadaLogica* coord = new CoordenadaLogica(siguiente->x, y_inicial);
					delete siguiente;
					siguiente = coord;
				}
				break;			
			
			case PATADAALTAAGACHADO:
			case PATADABAJAAGACHADO:
				//SDL_Delay(150);
				if (this->accionActual->ciclos == 1){
					Abajo = false;
					PatadaBaja = false;
					PatadaAlta = false;				
					cambiarAccionA(AGACHARSE);	
					this->accionActual->setModoActual(this->accionActual->cantModos-1);
				}				
				break;					
			case PATADAALTA:
			case PATADABAJA:
			case PINIABAJA:
			case PINIAALTA:
			case TRABA:								
				if (this->accionActual->ciclos == 1){
					cambiarAccionA(QUIETO);
				}
			break;			
			case PARARSE:
				if(this->accionActual->modoActual == 0){
					cambiarAccionA(QUIETO);	
				}
			break;
			case AGACHARSE:
				///puts("Holi");
				if(accion == QUIETO){
					cambiarAccionA(PARARSE);	
				}
				else if(accion == CUBRIRBAJO){
					puts("de agacharse a cubrirse"); ///
					cambiarAccionA(CUBRIRBAJO);
				}
				else if(accion == PATADABAJAAGACHADO){
					puts("de agacharse a patear");	///
					cambiarAccionA(PATADABAJAAGACHADO);
				}
				else if (accion == PATADAALTAAGACHADO){
					puts("de agachado a patada alta"); ///
					cambiarAccionA(PATADAALTAAGACHADO);
				}
				break;
			//	DEBERIA SER 'CAMINAR ATRAS' 
			/* PJ --> si mirando a derecha --> si izq + patada baja --> traba
			 * 		--> si mira a izquierda --> si der + patada baja --> traba
			 * */
			case CAMINAR_IZQUIERDA:
				if(accion == PATADABAJA){
					puts("De caminar izquierda a Traba"); ///
					cambiarAccionA(TRABA);
				}
				break;
			case CUBRIRBAJO:
				cambiarAccionA(CUBRIRBAJO);
				break;	
			default:
				break;
		}
	}
	
	 this->imagenActual = this->accionActual->getImagenActual();
	 //return;
	 
	switch(nroAccionActual){
		case SALTARDIAGONAL_DER:
			if (accion == PINIAALTA || accion == PINIABAJA){
				cambiarAccionA(PINIASALTANDODIAGONAL);								
				cout<<"SALTO DIAGONAL CON PINIA"<<endl; ///
			}else if (accion == PATADAALTA || accion == PATADABAJA){
				cambiarAccionA(PATADASALTANDODIAGONAL);
				cout<< "SALTO DIAGONAL CON PATADA"<<endl; ///
			}
			break;
		case SALTARDIAGONAL_IZQ:
			if (accion == PINIAALTA || accion == PINIABAJA){
				cambiarAccionA(PINIASALTANDODIAGONAL);				
			}else if (accion == PATADAALTA || accion == PATADABAJA){
				cambiarAccionA(PATADASALTANDODIAGONAL);
			}			
			break;
		case SALTAR:
			if (accion == PINIAALTA || accion == PINIABAJA){				
				cambiarAccionA(PINIASALTANDOVERTICAL);
			}else if (accion == PATADAALTA || accion == PATADABAJA){
				cambiarAccionA(PATADASALTANDOVERTICAL);
			}else if (accion == ARROJARARMA){
				cout<< "SALTO VERTICAL + ARROJO ARMA"<<endl; ///
			}
			break;
	}
	this->imagenActual = this->accionActual->getImagenActual();

	if(this->accionActual->accionNro == 5){
		cout<<"modo actual que se mostro: "<<this->accionActual->modoActual+1<<endl;
	}
	
}

CoordenadaLogica* Personaje::obtenerCoordenadaIzqSup(){
	CoordenadaLogica* coord = new CoordenadaLogica(coordenada);
	coord->desplazarY(alto);
	return coord;
}

CoordenadaLogica* Personaje::obtenerCoordenadaIzqInf(){
	CoordenadaLogica* coord = new CoordenadaLogica(coordenada);
	return coord;
}

CoordenadaLogica* Personaje::obtenerCoordenadaDerSup(){
	CoordenadaLogica* coord = new CoordenadaLogica(coordenada);
	coord->desplazarY(alto);
	coord->desplazarX(ancho);
	return coord;
}

CoordenadaLogica* Personaje::obtenerCoordenadaDerInf(){
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
			this->accionActual = this->estado->piniaAlta;
			break;
		case PINIABAJA:
			this->accionActual = this->estado->piniaBaja;
			break;
		case PATADAALTA:
			this->accionActual = this->estado->patadaAlta;
			break;
		case PATADABAJA:
			this->accionActual = this->estado->patadaBaja;
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
			cout<<"gancho"<<endl; ///
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
		default: // case SALTARDIAGONAL_IZQ:
			this->accionActual = this->estado->saltardiagonal;
			if(this->mirarDerecha){
				this->accionActual->setInvertirSecuencia();
			}
			this->accionActual->setDireccionIzquierda();
			break;
	}
		this->accionActual->saque_vida = false;
	
}


void Personaje::Dibujarse(){
	CoordenadaLogica* coord1 = this->obtenerCoordenadaIzqInf();
	CoordenadaFisica* coord1_fis = this->conversor->aFisica(coord1);
	
	CoordenadaLogica* coord2 = this->obtenerCoordenadaDerSup();
	CoordenadaFisica* coord2_fis = this->conversor->aFisica(coord2);
	
	int ancho_fisico = abs(coord1_fis->x_fisico - coord2_fis->x_fisico);		// Función de std
	int alto_fisico = abs(coord1_fis->y_fisico - coord2_fis->y_fisico);
	

	int _w, _h;
	
	SDL_QueryTexture(this->imagenActual, NULL, NULL, &_w, &_h);
	if (this->altura_quieto == 0 && this->nroAccionActual == 0) this->altura_quieto = (float) _h;
	if (this->ancho_quieto == 0 && this->nroAccionActual == 0) this->ancho_quieto = (float) _w;

	//Rectangulo destino
	SDL_Rect destino;
	destino.x = coord1_fis->x_fisico;
	if (!this->mirarDerecha && nroAccionActual != QUIETO) destino.x = coord1_fis->x_fisico + (this->ancho_quieto - _w)*this->conversor->factor_ancho;
	destino.y = coord2_fis->y_fisico + (this->altura_quieto - _h)*this->conversor->factor_alto;
	//destino.w = (_w)*this->conversor->factor_ancho;//ancho_fisico;
	destino.w = (_w)/this->ancho_quieto*ancho_fisico;//ancho_fisico;
	destino.h = (_h / this->altura_quieto)*alto_fisico; //

	SDL_Point point = {_w/2, _h};
	
	
	// Espeja si debe mirar para la izquierda.
	if (!this->mirarDerecha){
		
		SDL_RenderCopyEx(this->renderer, this->imagenActual, NULL, &destino,0,NULL,SDL_FLIP_HORIZONTAL);
	} else {
		
		SDL_RenderCopyEx(this->renderer, this->imagenActual, NULL, &destino,0,NULL,SDL_FLIP_NONE);
	}


	for(int i = 0; i < this->accionActual->rectangulos->size(); i++) {
		// Para evitar hacer esto acá podría crear un objeto
		// pero no quiero hacer una clase solo para este loop
		this->accionActual->rectangulos->at(i)->generar_rectanguloSDL(destino.x, destino.y, destino.w, destino.h ,renderer, !mirarDerecha);
			
	}
	
	delete coord1;
	delete coord1_fis;
	delete coord2;
	delete coord2_fis;
	
	if (this->arrojable){

		//SDL_RenderCopyEx(this->renderer, this->imagenActual, NULL, &destino,0,&point,SDL_FLIP_NONE);

		this->arrojable->dibujar(this->conversor);
	}
}


/***********************************************************************
 * 
 * 							CONTROLADOR
 *
 **********************************************************************/  
 void Personaje::ActualizarControlador(SDL_Joystick *joystick, Parser* conf){
	if (joystick == NULL)
		return;
	 
	int x_Joystick = SDL_JoystickGetAxis(joystick, 0);
	int y_Joystick = SDL_JoystickGetAxis(joystick, 1);
	
//Horizontal
	if( x_Joystick < -JOYSTICK_DEAD_ZONE ){		//  x = -1;		
		Izquierda = true;		
	}else if( x_Joystick > JOYSTICK_DEAD_ZONE ){//  x =  1;		
		Derecha = true;			
	}else{	//  x = 0;				
		Izquierda = false;
		Derecha = false;
	}
//Vertical
	if( y_Joystick < -JOYSTICK_DEAD_ZONE ){ //  y = -1;		
		Arriba = true;		
	}else if( y_Joystick > JOYSTICK_DEAD_ZONE ){ //y =  1;		
		Abajo = true;		
	}else{ //yDir = 0;		
		Arriba = false;
		Abajo = false;
	}	 
	
	unordered_map <string, int>* conf_joys = conf->joysticks->at(this->numero_jugador);
	for ( int i=0; i < SDL_JoystickNumButtons ( joystick ); ++i ){
		unsigned int boton = SDL_JoystickGetButton ( joystick, i );
		if ( boton != 0 ){
			// Aca había un swich case que Volpe sacó
			// porque solo interpreta constantes y no
			// podía cargar las configuraciones
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
				this->Arrojar();
			} else if (i == (*conf_joys)["arrojar_arma_baja"]){
				//ArrojarArma = true;
				this->Arrojar();
				this->arrojable->tirarDiagonal(TIRAR_ARRIBA);
			} else if (i == (*conf_joys)["arrojar_arma_alta"]){
				//ArrojarArma = true;
				this->Arrojar();
				this->arrojable->tirarDiagonal(TIRAR_ABAJO);
			} else if (i == (*conf_joys)["patada_alta"]){
				PatadaAlta = true;
			}
				
			///cout <<"Apretado boton "<< i <<endl; ///				
			
		}else{
			//Si no se aprieto boton --> todos en falso.
			//PiniaBaja = Cubrirse = PatadaBaja = PiniaAlta = ArrojarArma = PatadaAlta = false;			
		}
		//Si ya estaba apretado lo dejo.
		if (SDL_JoystickGetButton(joystick,1) == (*conf_joys)["cubrirse"]){
			CubrirAlto = true;
			///cout<<"cubriendose"<<endl; ///
		}else{
			CubrirAlto = false;
		}
		
	}
	 
}
