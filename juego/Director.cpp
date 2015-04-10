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

void Director::verificar_movimientos(){
	// Verificar en cada uno si debería scrollear, o si debería quedarse donde está.
	CoordenadaLogica* coord1 = jugadores[jugador1]->obtenerSiguienteCoordenadaDerSup();
	if (
	// Caso: scrollear a la derecha.
	if (ventana.coordenadaEnPantalla(coord1) == bordeDer){
		if (sePuedeScrollearDerecha()){
			//scrollear derecha
		} else {
			CoordenadaLogica* bordeDer = ventana->obtenerBordeLogicoDerecho(conversor);
			
			jugadores[jugador1]->moverseA();
		}
			
	}
	
	// Caso: scrollear a la izquierda.
	delete coord1;
	coord1 = jugadores[jugador1]->obtenerSiguienteCoordenadaIzqSup();
	if (ventana.coordenadaEnPantalla(coord1) == bordeIzq) {
		//scrollear izquierda
	}
	
	// Caso: la posición era válida.
	jugadores[jugador1]->moverseAIzqSup(coord1);
}


/* Recibe cuánto scrollear (o es siempre constante?????)
 * Si es positivo es para la derecha; negativo para la izquierda. */
 // En píxeles, lógicas??
bool Director::ventanaPuedeScrollear(???){
	
}

//~ /* Devuelve la misma coordenada si es válida.
 //~ * Sino, libera la anterior, crea una nueva con un lugar válido, dentro del margen. */
//~ CoordenadaLogica* Director::validar_lugar(CoordenadaLogica* coord, float ancho_personaje){
	//~ coord.desplazarX(ancho_personaje/2); // Esquina Derecha
	//~ coord_fisica = conversor.aFisica(coord);
	//~ if (ventana.coordenadaEnPantalla(coord_fisica) == bordeDer){
		//~ delete coord_fisica;
		//~ delete coord;
		//~ return ventana.obtenerBordeLogicoDerecho(conversor);
	//~ }
	//~ coord.desplazarX(-ancho_personaje); // Esquina Izquierda
	//~ coord_fisica = conversor.aFisica(coord);
	//~ if (ventana.coordenadaEnPantalla(coord_fisica) == bordeIzq){
		//~ delete coord_fisica;
		//~ delete coord;
		//~ return ventana.obtenerBordeLogicoIzquierdo(conversor);
	//~ }	
	//~ return coord.desplazarX(ancho_personaje/2); // Volvió a como estaba.
//~ }
//~ 
//~ void Director::verificar_movimiento_y_scroll(){
	//~ verificar_necesidad_de_scroll();
	//~ if (scroll1 && scroll2 && mov1 != mov2){
		//~ //Accionan pero
		//~ //Se quedan en donde están.
		//~ //No scrollea
	//~ } else if (scroll1 || scroll2){
		//~ //Accionar
		//~ //Mover
		//~ //Scrollear
	//~ } else {
		//~ //Accionar
		//~ //Mover
		//~ //No scrollear
	//~ }		
//~ }
//~ 
//~ CoordenadaLogica* analizar_scroll
//~ 
//~ void Director::verificar_necesidad_de_scroll_y_accionar_movimientos(){
	//~ switch(mov1){
		//~ case Derecha:
			//~ if (ventana.coordenadaEnPantalla(jugadores[jugador1]->obtenerCoordenadaDerSup()) == BordeDer){
				//~ scroll1 = true;
				//~ nueva_coord1 = jugadores[jugador1]->caminarDerecha();
			//~ }
			//~ break;
		//~ case Izquierda:
			//~ if (ventana.coordenadaEnPantalla(jugadores[jugador1]->obtenerCoordenadaIzqSup()) == BordeIzq){
				//~ scroll1 = true;
				//~ nueva_coord1 = jugadores[jugador1]->caminarIzquierda();
			//~ }
			//~ break;
		//~ case Arriba:
			//~ scroll1 = false;
			//~ jugadores[jugador1]->saltar();
			//~ break;
		//~ default: //case Nada:
			//~ scroll1 = false;
			//~ nueva_coord1 = jugadores[jugador1]->parar();
			//~ break;
	//~ }
	//~ switch(mov2){
		//~ case Derecha:
			//~ if (ventana.coordenadaEnPantalla(jugadores[jugador2]->obtenerCoordenadaDerSup()) == BordeDer)
															//~ // u obtenerCoordenadaDerInf()
				//~ scroll2 = true;
			//~ break;
		//~ case Izquierda:
			//~ if (ventana.coordenadaEnPantalla(jugadores[jugador2]->obtenerCoordenadaIzqSup()) == BordeIzq)
															//~ // u obtenerCoordenadaIzqInf()
				//~ scroll2 = true;
		//~ default:
			//~ break;
	//~ }
//~ }

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

