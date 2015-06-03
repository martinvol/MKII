#ifndef AI_H
#define AI_H

#include "../Personaje/Personaje.hpp"
#include "../Personaje/Acciones/Accion.hpp"

typedef enum {CLOSE, MID, FAR} Rg;

class AI {
	private:
		//accion_posible accActH;
		accion_posible accAntH;
		accion_posible accAntS;
		Rg where;
		//accion_posible accActS;
		bool updateH(); 
		bool updateS();	
		bool updateWhere();
	public:
		Personaje* personajeAI;
		Personaje* personajeHumano;
		
		void reaccionar();
		
		AI(Personaje* personajeAI, Personaje* personajeHumano);
		// ~AI();
};

#endif
