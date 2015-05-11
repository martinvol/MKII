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

Personaje::Personaje(CoordenadaLogica* coord, string nombre,SDL_Renderer* ren, float alto, float ancho, Estado* estado, ConversorDeCoordenadas* conversor){
	// 4 flechas
	Nada = Izquierda = Derecha = Arriba = Abajo = false;
	// 2 pinias
	PiniaAlta = PiniaBaja = false;
	// 2 patadas
	PatadaAlta = PatadaBaja = false;
	// arrojable y cubrirse
	ArrojarArma = Cubrirse = false;
	
	this->alto = alto;
	this->ancho = ancho;
	this->conversor = conversor;
	this->y_inicial = coord->y;
	this->coordenada = coord;
	this->siguiente = NULL;
	
	this->mirarDerecha = false;
	
	this->estado = estado;
	this->nroAccionActual = QUIETO;
	this->accionActual = this->estado->	quieto;
	this->imagenActual = this->accionActual->getImagenActual();
	
	this->nombrePersonaje = nombre;
	this->renderer = ren;
	
}

Personaje::~Personaje(){
	delete this->coordenada;
	delete this->estado;	// Esto elimina la acción y sus imágenes.
}

void Personaje::Arrojar(){
	cout << "el personaje sabe que tiene que arrojar el arma" << endl; 
	arrojable = new Arrojable("/home/martin/Desktop/evilFighter.png", true, this->renderer);

	//arrojable->setCoordenadas(new CoordenadaLogica(this->coordenada->x, this->coordenada->y), this->alto, this->ancho);
	arrojable->setCoordenadas(new CoordenadaLogica(obtenerCoordenadaDerSup()), this->alto, this->ancho);
	arrojable->tirar();

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
		if (siguiente != NULL){ delete siguiente; }
		siguiente = this->accionActual->execute(this->coordenada);
		switch (nroAccionActual){
			case SALTAR:
			case SALTARDIAGONAL_DER:
			case SALTARDIAGONAL_IZQ:
				if (siguiente->y < y_inicial){
					cambiarAccionA(QUIETO);
					CoordenadaLogica* coord = new CoordenadaLogica(siguiente->x, y_inicial);
					delete siguiente;
					siguiente = coord;
				}
				break;
			case PINIABAJA:
			case PINIAALTA:
				if (this->accionActual->esUltimoModo()){
					cambiarAccionA(QUIETO);
				}
			
			//~ case MIRARIZQUIERDA:
				//~ if(this->accionActual->esUltimoModo()){
					//~ puts ("holi");
					//~ cambiarAccionA(QUIETO);
				//~ }
			
			default:
				break;
		}
	}
	
	/// MILE!!!
	/* De las acciones se pueden interrumpir los saltos: con pinias, 
	 * patadas y el arrojable.
	 * Ahora 'andan' los saltos con pinias (o sea, muestra el cout).
	 * 
	 * */
	switch(nroAccionActual){		
		case SALTARDIAGONAL_DER:
			if (accion == PINIAALTA || accion == PINIABAJA){				
				cout<<"SALTO DIAGONAL CON PINIA"<<endl; ///
			}else if (accion == PATADAALTA || accion == PATADABAJA){
				cout<< "SALTO DIAGONAL CON PATADA"<<endl; ///
			}
			break;
		case SALTARDIAGONAL_IZQ:
			if (accion == PINIAALTA || accion == PINIABAJA){				
				cout<<"SALTO CON PINIA"<<endl; ///
			}else if (accion == PATADAALTA || accion == PATADABAJA){
				cout<< "SALTO CON PATADA"<<endl; ///
			}			
			break;
		case SALTAR:
			if (accion == PINIAALTA || accion == PINIABAJA){				
				cout<<"SALTO VERTICAL CON PINIA"<<endl; ///
			}else if (accion == PATADAALTA || accion == PATADABAJA){
				cout<< "SALTO VERTICAL CON PATADA"<<endl; ///
			}else if (accion == ARROJARARMA){
				cout<< "SALTO VERTICAL + ARROJO ARMA"<<endl; ///
			}
			break;			
	}
//~ if (nroAccionActual == AGACHARSE)
//~ cout<<"quiero agacharme"<<endl;
	this->imagenActual = this->accionActual->getImagenActual();

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
	if (siguiente == NULL) return NULL;
	CoordenadaLogica* coord = new CoordenadaLogica(siguiente);
	coord->desplazarY(alto);
	return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaIzqInf(){
	if (siguiente == NULL) return NULL;
	CoordenadaLogica* coord = new CoordenadaLogica(siguiente);
	return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaDerSup(){
	if (siguiente == NULL) return NULL;
	CoordenadaLogica* coord = new CoordenadaLogica(siguiente);
	coord->desplazarY(alto);
	coord->desplazarX(ancho);
	return coord;
}

CoordenadaLogica* Personaje::obtenerSiguienteCoordenadaDerInf(){
	if (siguiente == NULL) return NULL;
	CoordenadaLogica* coord = new CoordenadaLogica(siguiente);
	coord->desplazarX(ancho);
	return coord;
}

void Personaje::moverseAIzqSup(CoordenadaLogica* coord){
	if (!this->coordenada) delete coordenada;
	coordenada = new CoordenadaLogica(coord);
	coordenada->desplazarY(-alto);
}

void Personaje::moverseADerSup(CoordenadaLogica* coord){
	if (!this->coordenada) delete coordenada;
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
	
	switch (nroAccionActual)
	{ 
		case QUIETO:
			puts( "y entre a quieto");
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
		default: // case SALTARDIAGONAL_IZQ:
			this->accionActual = this->estado->saltardiagonal;
			if(this->mirarDerecha){
				this->accionActual->setInvertirSecuencia();
			}
			this->accionActual->setDireccionIzquierda();
			break;
	}
	
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

	//Rectangulo destino
	SDL_Rect destino;
	destino.x = coord1_fis->x_fisico;
	if (!this->mirarDerecha) destino.x = coord1_fis->x_fisico - _w;
	destino.y = coord2_fis->y_fisico;
	destino.w = (_w)*this->conversor->factor_ancho;//ancho_fisico;
	destino.h = alto_fisico; //

	SDL_Point point = {_w/2, _h};
	
	for(int i = 0; i < this->accionActual->rectangulos->size(); i++) {
		// Para evitar hacer esto acá podría crear un objeto
		// pero no quiero hacer una clase solo para este loop
		this->accionActual->rectangulos->at(i)->generar_rectanguloSDL(destino.x, destino.y, destino.w, destino.h ,renderer, !mirarDerecha);
			
	}

	// Espeja si debe mirar para la izquierda.
	if (!this->mirarDerecha){
		SDL_RenderCopyEx(this->renderer, this->imagenActual, NULL, &destino,0,&point,SDL_FLIP_HORIZONTAL);
	} else {
		SDL_RenderCopyEx(this->renderer, this->imagenActual, NULL, &destino,0,&point,SDL_FLIP_NONE);
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
	
	for ( int i=0; i < SDL_JoystickNumButtons ( joystick ); ++i ){
		unsigned int boton = SDL_JoystickGetButton ( joystick, i );
		if ( boton != 0 ){
			// Aca había un swich case que Volpe sacó
			// porque solo interpreta constantes y no
			// podía cargar las configuraciones
			if (conf->pinia_baja){
				PiniaBaja = true;					
			}
			else if (conf->pinia_alta){
				Cubrirse = true;
			}
			else if (conf->patada_baja){
				PatadaBaja = true;
			}
			else if (conf->pinia_alta){
				PiniaAlta = true;
			}
			else if (conf->arrojar_arma){
				ArrojarArma = true;
			}
			else if (conf->patada_alta){
				PatadaAlta = true;
			}
				
			cout <<"Apretado boton "<< i <<endl; ///				
			
		}else{
			//Si no se aprieto boton --> todos en falso.
			//PiniaBaja = Cubrirse = PatadaBaja = PiniaAlta = ArrojarArma = PatadaAlta = false;			
		}
		//Si ya estaba apretado lo dejo.
		if (SDL_JoystickGetButton(joystick,1) == 1){
			Cubrirse = true;
			cout<<"cubriendose"<<endl; ///
		}else{
			Cubrirse = false;
		}
		
	}
	 
}
