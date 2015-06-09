//~ #include <locale>
#include "ControladorTextBox.hpp"

ControladorTextBox::ControladorTextBox(TextBox* textbox){
	SDL_StartTextInput();
	this->textbox = textbox;
}

ControladorTextBox::~ControladorTextBox(){
	SDL_StopTextInput();
	// No debe destruir el textbox
}

void ControladorTextBox::procesarEvento(SDL_Event* evento){
	//~ SDL_EnableUNICODE(SDL_ENABLE);
	switch (evento->type){
		case SDL_KEYDOWN:
			//Borrar
			if( evento->key.keysym.sym == SDLK_BACKSPACE)
			{
				textbox->sacarCaracter();
			}
			//Copiar
			else if( evento->key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
			{
				SDL_SetClipboardText(textbox->obtenerTexto().c_str());
			}
			//Pegar
			else if( evento->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
			{
				textbox->agregarTexto(SDL_GetClipboardText());
			}
			//~ } else {
				//~ char c = event->key.keysym.unicode;
				//~ if (isprint(c)) textbox->agregarCaracter(c);
			//~ }
			break;
		case SDL_TEXTINPUT:
			//No copiar y pegar
			if( !(
					( ( evento->text.text[ 0 ] == 'c' || evento->text.text[ 0 ] == 'C' ) || 
					( evento->text.text[ 0 ] == 'v' || evento->text.text[ 0 ] == 'V' ) ) && 
					SDL_GetModState() & KMOD_CTRL )
			) {
				textbox->agregarTexto(evento->text.text);
			}
			break;
		default:
			;
	}
}
