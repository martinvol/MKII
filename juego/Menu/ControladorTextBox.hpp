#ifndef CONT_TEXTBOX_H_INCLUDED
#define CONT_TEXTBOX_H_INCLUDED

#include "TextBox.hpp"

class ControladorTextBox {
	
	private:
		TextBox* textbox;
		
	public:
		ControladorTextBox(TextBox* textbox);
		~ControladorTextBox();
		void procesarEvento(SDL_Event* event);

};

#endif //CONT_TEXTBOX_H_INCLUDED

