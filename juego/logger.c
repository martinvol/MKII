#include <cstdio>
#include <string>
#include "logger.h"
#include <iostream>

using namespace std;

Logger::Logger(){
	log_debug("Logger inicializado");
}

Logger* Logger::instance(){
		static Logger myinstance;
		return &myinstance;
	}

void Logger::log_debug(std::string const &message){
	if (debug){
		printf("\x1b[%dm", 32); // imprime en color verde
		cout<<"[DEBUG] " << message<<endl;
		printf("\x1b[%dm", 0);  // Vuelve al color estandar
	}
}
void Logger::log_warning(std::string const &message){
	if (warning){
		printf("\x1b[%dm", 33); // imprime en color amarillo
		cout<<"[WARNING] " << message<<endl;
		printf("\x1b[%dm", 0); // Vuelve al color estandar
	}
}
void Logger::log_error(std::string const &message){
	if (error){
		printf("\x1b[%dm", 31); // imprime en color rojo
		cout<<"[ERROR] " << message<<endl;
		printf("\x1b[%dm", 0); // Vuelve al color estandar
	}
}


void Logger::set_debug(bool nuevo_stado){
	debug = nuevo_stado;
}
void Logger::set_warning(bool nuevo_stado){
	warning = nuevo_stado;
}
void Logger::set_error(bool nuevo_stado){
	error = nuevo_stado;
}