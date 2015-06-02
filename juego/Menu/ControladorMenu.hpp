#ifndef CONT_MENU_H
#define CONT_MENU_H

#include "Menu.hpp"

class ControladorMenu{
	private:
	
	public:
		bool arriba, abajo, enter;
		Menu* menu;
		
		ControladorMenu(Menu* menu);
		modo procesarEvento(SDL_Event* evento);
	
};
#endif //CONT_MENU_H
