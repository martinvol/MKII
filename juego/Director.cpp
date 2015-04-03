#include "Director.h"

void Director::analizarMultieventoDeUnJugador(movimiento* mov, movimiento lugar){
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

void Director::seMuevePersonaje(num_jugador jugador, movimiento lugar){
	switch (jugador){
		case jugador1:
			analizarMultieventoDeUnJugador(&mov1, lugar);
			break;
		default: //jugador2
			analizarMultieventoDeUnJugador(&mov2, lugar);
			break;
	}
}

/* Devuelve la misma coordenada si es válida.
 * Sino, libera la anterior, crea una nueva con un lugar válido, dentro del margen. */
CoordenadaLogica* Director::validar_lugar(CoordenadaLogica* coord, float ancho_personaje){
	coord.desplazarX(ancho_personaje/2); // Esquina Derecha
	coord_fisica = conversor.aFisica(coord);
	if (ventana.coordenadaEnPantalla(coord_fisica) == bordeDer){
		delete coord_fisica;
		delete coord;
		return ventana.obtenerBordeLogicoDerecho(conversor);
	}
	coord.desplazarX(-ancho_personaje); // Esquina Izquierda
	coord_fisica = conversor.aFisica(coord);
	if (ventana.coordenadaEnPantalla(coord_fisica) == bordeIzq){
		delete coord_fisica;
		delete coord;
		return ventana.obtenerBordeLogicoIzquierdo(conversor);
	}	
	return coord.desplazarX(ancho_personaje/2); // Volvió a como estaba.
}

void Director::actualizar(){
	verificar_movimiento();
	verificar_orientaciones();
}

void Director::verificar_movimiento_y_scroll(){
	verificar_necesidad_de_scroll();
	if (scroll1 && scroll2 && mov1 != mov2){
		//Accionan pero
		//Se quedan en donde están.
		//No scrollea
	} else if (scroll1 || scroll2){
		//Accionar
		//Mover
		//Scrollear
	} else {
		//Accionar
		//Mover
		//No scrollear
	}		
}

CoordenadaLogica* analizar_scroll

void Director::verificar_necesidad_de_scroll_y_accionar_movimientos(){
	switch(mov1){
		case Derecha:
			if (ventana.coordenadaEnPantalla(jugadores[jugador1]->obtenerCoordenadaDerSup()) == BordeDer){
				scroll1 = true;
				nueva_coord1 = jugadores[jugador1]->caminarDerecha();
			}
			break;
		case Izquierda:
			if (ventana.coordenadaEnPantalla(jugadores[jugador1]->obtenerCoordenadaIzqSup()) == BordeIzq){
				scroll1 = true;
				nueva_coord1 = jugadores[jugador1]->caminarIzquierda();
			}
			break;
		case Arriba:
			scroll1 = false;
			jugadores[jugador1]->saltar();
			break;
		default: //case Nada:
			scroll1 = false;
			nueva_coord1 = jugadores[jugador1]->parar();
			break;
	}
	switch(mov2){
		case Derecha:
			if (ventana.coordenadaEnPantalla(jugadores[jugador2]->obtenerCoordenadaDerSup()) == BordeDer)
															// u obtenerCoordenadaDerInf()
				scroll2 = true;
			break;
		case Izquierda:
			if (ventana.coordenadaEnPantalla(jugadores[jugador2]->obtenerCoordenadaIzqSup()) == BordeIzq)
															// u obtenerCoordenadaIzqInf()
				scroll2 = true;
		default:
			break;
	}
}

void Director::verificar_orientaciones(){
	CoordenadaLogica* coord1 = jugadores[jugador1]->obtenerCoordenada();
	CoordenadaLogica* coord2 = jugadores[jugador2]->obtenerCoordenada();
	if (coord1->estaALaDerechaDe(coord2)){
		jugadores[jugador1]->mirarParaIzquierda();
		jugadores[jugador2]->mirarParaDerecha();
	} else {
		jugadores[jugador1]->mirarParaDerecha();
		jugadores[jugador2]->mirarParaIzquierda();
	}
}

