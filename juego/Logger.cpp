#include <cstdio>
#include <string>
#include "Logger.hpp"
#include <iostream>
#include <time.h>       /* time_t, struct tm, time, localtime, strftime */

using namespace std;

void tiempo(char* buffer){
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
 	timeinfo = localtime (&rawtime);
	strftime (buffer, 80, "%I:%M%p", timeinfo);
}

Logger::Logger(){
	log_debug("Logger inicializado");
}

Logger* Logger::myinstance = NULL;

Logger* Logger::instance(){
		if (myinstance == NULL) myinstance = new Logger();
		return myinstance;
	}

void Logger::destroy_instance(){
	if (myinstance != NULL){
		delete myinstance;
		myinstance = NULL;
	}
}

void Logger::log_debug(std::string const &message){
    char buffer [80];
    tiempo(buffer);

	if (debug){
		printf("\x1b[%dm", 32); // imprime en color verde
		cout <<"[DEBUG [" << buffer << "]] " << message<<endl;
		printf("\x1b[%dm", 0);  // Vuelve al color estandar
	}
}
void Logger::log_warning(std::string const &message){
	char buffer [80];
    tiempo(buffer);
	if (warning){
		printf("\x1b[%dm", 33); // imprime en color amarillo
		cout<<"[WARNING [" << buffer << "]] " << message<<endl;
		printf("\x1b[%dm", 0); // Vuelve al color estandar
	}
}
void Logger::log_error(std::string const &message){
	char buffer [80];
    tiempo(buffer);
	if (error){
		printf("\x1b[%dm", 31); // imprime en color rojo
		cout<<"[ERROR [" << buffer << "]] " << message<<endl;
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
