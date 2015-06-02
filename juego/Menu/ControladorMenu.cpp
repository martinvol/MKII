#include "ControladorMenu.hpp"

#include <vector>
#include <iostream>
ControladorMenu::ControladorMenu(Menu* menu){
	abajo = false;
	arriba = false;
	enter = false;
	this->menu = menu;
	en_boton = false;
}

modo ControladorMenu::procesarEvento(SDL_Event* evento){
	while(SDL_PollEvent(evento)) {
		switch (evento->type){
			case SDL_JOYAXISMOTION:
			    /* If Up-Down movement */
				if( evento->jaxis.axis == 1){
					int y_Joystick = evento->jaxis.value;
					if( y_Joystick < -JOYSTICK_DEAD_ZONE ){ //  y = -1;		
						if (arriba == false) menu->subirOpcion();
						arriba = true;
					}else if( y_Joystick > JOYSTICK_DEAD_ZONE ){ //y =  1;		
						if (abajo == false) menu->bajarOpcion();
						abajo = true;
					}else{ //yDir = 0;
						arriba = false;
						abajo = false;
					}
				}
				break;
			case SDL_KEYDOWN:
				if (evento->key.keysym.sym == SDLK_UP){
					if (arriba == false) menu->subirOpcion();
					arriba = true;
				} else
				if (evento->key.keysym.sym == SDLK_DOWN){
					if (abajo == false) menu->bajarOpcion();
					abajo = true;
				}
				if (evento->key.keysym.sym == SDLK_RETURN){
					enter = true;
				}
				break;
			case SDL_KEYUP:
				if (evento->key.keysym.sym == SDLK_UP){
					arriba = false;
				} else
				if (evento->key.keysym.sym == SDLK_DOWN){
					abajo = false;
				}
				break;
			case SDL_JOYBUTTONDOWN:
				if (evento->jbutton.button == 0){
					enter = true;
				}
				break;
			case SDL_MOUSEMOTION:
				if (evento->motion.windowID == menu->obtenerIDventana()){
					bool entro = false;
					vector<SDL_Rect> botones = menu->obtenerPosicionesBotones();
					for(int i=0;i<botones.size();i++){
						if ( (evento->motion.x >= (Sint32) botones[i].x) &&
							 (evento->motion.x <= (Sint32) (botones[i].x + botones[i].w)) &&
							 (evento->motion.y >= (Sint32) botones[i].y) &&
							 (evento->motion.y <= (Sint32) (botones[i].y + botones[i].h))
						){
							menu->apuntarAOpcion((modo) i);
							entro = true;
						}
					}
					if (entro) en_boton = true;
					else en_boton = false;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (evento->button.windowID == menu->obtenerIDventana()){
					if (en_boton) enter = true;
				}
				break;
			default:
				;
		}
	}
	if (enter == true){
		return menu->seleccionarOpcion();
	}
	return MENU;
}
