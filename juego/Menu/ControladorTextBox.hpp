#ifndef CONT_TEXTBOX_H_INCLUDED
#define CONT_TEXTBOX_H_INCLUDED

#include "TextBox.hpp"

class ControladorTextBox {
	
	private:
		TextBox* textbox;
		bool en_textbox;
		Uint32 idVentana;
		
	public:
		ControladorTextBox(TextBox* textbox, Uint32 idVentana);
		~ControladorTextBox();
		void procesarEvento(SDL_Event* event);
		void resetear();
};

#endif //CONT_TEXTBOX_H_INCLUDED

