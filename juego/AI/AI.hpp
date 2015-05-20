#ifndef AI_H
#define AI_H

#include "../Personaje/Personaje.hpp"

class AI {
	public:
		Personaje* personajeAI;
		Personaje* personajeHumano;
		
		void reaccionar();
		
		AI(Personaje* personajeAI, Personaje* personajeHumano);
		// ~AI();
};

#endif
