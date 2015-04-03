#include "Director.h"

void Director::seMuevePersonaje(num_jugador jugador, movimiento lugar){
	switch (jugador){
		case jugador1:
			this->mov1 = lugar;
			break;
		default:
			this->mov2 = lugar;
			break;
	}
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

void Director::verificar_necesidad_de_scroll_y_accionar_movimientos(){
	switch(mov1){
		case Derecha:
			if (ventana.coordenadaEnPantalla(jugadores[jugador1]->obtenerCoordenadaDerSup()) == BordeDer)
															// u obtenerCoordenadaDerInf()
				scroll1 = true;
				nueva_coord1 = jugadores[jugador1]->caminarDerecha();
			break;
		case Izquierda:
			if (ventana.coordenadaEnPantalla(jugadores[jugador1]->obtenerCoordenadaIzqSup()) == BordeIzq)
															// u obtenerCoordenadaIzqInf()
				scroll1 = true;
				nueva_coord1 = jugadores[jugador1]->caminarIzquierda();
			break;
		case Arriba:
			scroll1 = false;
			jugadores[jugador1]->saltar();
			break;
		default: //case Nada:
			nueva_coord1 = scroll1 = false;
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

