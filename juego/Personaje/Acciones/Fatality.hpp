#ifndef FATALITY_H_INCLUDED
#define FATALITY_H_INCLUDED

class Fatality {
	Personaje* victima;
	Personaje* victimario;
	Perform* perform;
	public:
		void execute();
};

#endif
