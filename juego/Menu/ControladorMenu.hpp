#ifndef CONT_MENU_H
#define CONT_MENU_H

#include "Menu.hpp"

#define JOYSTICK_DEAD_ZONE 10000

class ControladorMenu{
	private:
	
	public:
		bool arriba, abajo, enter, en_boton;
		Menu* menu;
		
		ControladorMenu(Menu* menu);
		modo procesarEvento(SDL_Event* evento);
	
};
#endif //CONT_MENU_H
