#include <cstdio>
#include <cstring>
#include <fstream>
#include <unordered_map>
#include "Capa.h"
#include "parser.h"
#include "logger.h"
#include "jsoncpp/json/json.h"

#define IMAGEN_DEFAULT "resources/background/defaultcapa.png"
#define DEFAULT_X_LOGICO 0

using namespace std;

bool exists_test(const std::string& name) {
    ifstream f(name.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }   
}

void Conf::set_values (char* my_file) {
    logger = Logger::instance();
    logger->log_debug("Inicializando parser");

    logger->log_debug(std::string("Intenta cargar configuraciones desde ") +  my_file);

    Json::Value root;
    Json::Reader reader;

    if (my_file == NULL){
            logger->log_error("Problema con el archivo, probablemente no existe");
            cargarDefault();
    }
    
    std::ifstream test(my_file, std::ifstream::binary);

    if (!test.is_open()){
        logger->log_error("Problema con el archivo, probablemente no existe");
        cargarDefault();

    }

    else {

        bool parsingSuccessful = reader.parse(test, root, false);

        if (parsingSuccessful){

            const Json::Value ventana = root["ventana"];
            
            ventana_anchopx = cargarValidar(ventana, 800, "anchopx", "Usando anchopx default de 800px");
            ventana_altopx = cargarValidar(ventana, 416, "altopx", "Usando altopx default de 416px");

            ventana_ancho = cargarValidar(ventana, 600, "ancho", "Usando ancho default de 600px");
            margen = cargarValidar(ventana, 80, "margen", "Usando marge default de 80%");

            const Json::Value escenario = root["escenario"];

            escenario_ancho = cargarValidar(escenario, 2000, "ancho", "Usando ancho default de 2000");
            escenario_alto = cargarValidar(escenario, 200, "alto", "Usando ancho default de 200");
            escenario_ypiso = cargarValidar(escenario, 0, "ypiso", "Usando ypiso default de 0");


            const Json::Value personaje = root["personaje"];

            personaje_ancho = cargarValidar(personaje, 100, "ancho", "Usando ancho (personaje) default de 100");
            personaje_alto = cargarValidar(personaje, 100, "alto", "Usando alto (personaje) default de 100");
            personaje_zindex = cargarValidar(personaje, 0, "zindex", "Usando zindex (personaje) default de 0");

            const Json::Value sprites = personaje["sprites"];

            for (auto const& id : sprites.getMemberNames()) {
                sprites_map[id] = sprites.get(id, "Esto nunca se va a mostrar").asString();
            }

            const Json::Value capas = root["capas"];
            
            for ( int index = 0; index < capas.size(); ++index ){
                
                string nombre_archivo;

                if (!capas[index].isMember("imagen_fondo")){
                    logger->log_error("Esta capa no tiene el valor imagen_fondo, se cargará la capa por default");
                }
                
                nombre_archivo = capas[index].get("imagen_fondo", IMAGEN_DEFAULT).asString(); // este default hay que ponerlo bien
                logger->log_debug("Intentando cargar capa " + nombre_archivo);
                
                if (!exists_test(nombre_archivo)){
                    logger->log_error("La capa no fue encontrada, cargando capa por default");
                    nombre_archivo = IMAGEN_DEFAULT;
                }

                Capa *temp = new Capa(
                    nombre_archivo, 
                    capas[index].get("anchoLogico", 0).asFloat(),
                    DEFAULT_X_LOGICO, 
                    NULL,
                    NULL);

                capas_vector.push_back(temp);
            }

            personaje_mirar_derecha = cargarValidarBool(personaje, true, "mirar_derecha", "El personaje no tiene mirar_derecha, se carga por default derecha");

        } else {
            logger->log_error("Error de sytaxis en el archivo");
            logger->log_error(reader.getFormatedErrorMessages());
            cargarDefault();
        }
    }
}

float Conf::cargarValidar(Json::Value objetoJson, float valorDefault, char* clave, char* mensaje){
   if (!objetoJson.isMember(clave)){
        logger->log_warning(std::string("Json no tiene el parametro: ") + clave);
        logger->log_debug(mensaje);
    }
    return objetoJson.get(clave, valorDefault).asFloat();
}

bool Conf::cargarValidarBool(Json::Value objetoJson, bool valorDefault, char* clave, char* mensaje){
   if (!objetoJson.isMember(clave)){
        logger->log_warning(std::string("ventana no tiene el parametro: ") + clave);
        logger->log_debug(mensaje);
    }
    return objetoJson.get(clave, valorDefault).asBool();
}


void Conf::cargarDefault(){
    logger->log_debug("Cargando configuración default");
    Conf::set_values("resources/default.json");
}