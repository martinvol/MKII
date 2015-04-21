#include <sstream>
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

            const Json::Value escenario = root["escenario"];

            escenario_ancho = cargarValidar(escenario, 1500, "ancho", "Usando ancho default de 1500");

            if (escenario_ancho > 2000 || escenario_ancho < 1000){
                logger->log_error("El ancho del escenario es inválido, se usará el ancho por defautl, 1500");
                escenario_ancho = 1500;
            }

            escenario_alto = cargarValidar(escenario, 200, "alto", "Usando ancho default de 200");

            if (escenario_alto > (3./4.)*escenario_ancho || escenario_ancho < (1/4)){
                logger->log_error("El alto del escenario es inválido, se usará el ancho por default, la mitad del ancho");
                escenario_alto = escenario_ancho/2.;
            }

            escenario_ypiso = cargarValidar(escenario, 0, "ypiso", "Usando ypiso default de 0");


            const Json::Value ventana = root["ventana"];
            
            ventana_anchopx = cargarValidar(ventana, 800, "anchopx", "Usando anchopx default de 800px");
            if (ventana_anchopx > 1200 || ventana_anchopx<400){
                logger->log_error("El ancho de la ventana es inválido, se usará el ancho por defautl, 800px.");
                ventana_anchopx = 800;
            }

            ventana_altopx = cargarValidar(ventana, 416, "altopx", "Usando altopx default de 416px");
            if (ventana_altopx > 1000 || ventana_anchopx< 200){
                logger->log_error("El alto de la ventana es inválido, se usará el ancho por defautl, 600px.");
                ventana_altopx = 600;
            }


            ventana_ancho = cargarValidar(ventana, 600, "ancho", "Usando ancho default de 600px");
            if (ventana_ancho > 700 || ventana_ancho < 200){
                logger->log_error("El alto de la ventana es inválido, se usará el ancho por defautl, 600px.");
                ventana_ancho = 600;
            }
            
            margen = cargarValidar(ventana, 70, "margen", "Usando marge default de 70%");
            if (margen > 100){
                logger->log_error("El margen no puede ser mayor que 100, se usará el ancho por defautl, 70%.");
                margen = 70;
            }

            const Json::Value personaje = root["personaje"];

            personaje_ancho = cargarValidar(personaje, 100, "ancho", "Usando ancho (personaje) default de 100");
            if (personaje_ancho >(3./4)*ventana_ancho || personaje_ancho < 0){
                logger->log_error("El ancho del personaje es inválido, se usa un cuarto del ancho de la pantalla (logica).");
                personaje_ancho = ventana_ancho/4.;
            }

            personaje_alto = cargarValidar(personaje, 100, "alto", "Usando alto (personaje) default de 100");
            if (personaje_alto >(3./4)*escenario_alto || personaje_alto < (1./8)*escenario_alto){
                logger->log_error("El alto del personaje es inválido, se usa un cuarto del ancho de la pantalla (logica).");
                personaje_alto = escenario_alto/4.;
            }


            const Json::Value sprites = personaje["sprites"];

            for (auto const& id : sprites.getMemberNames()) {
                try{
                    sprites_map[id] = sprites.get(id, "Esto nunca se va a mostrar").asString();
                } catch(const runtime_error& error) {
                    logger->log_error("El path del personaje es un número, no un string");
                    sprites_map[id] = "resources/jugador/SubZero/";
                }
            }

            const Json::Value capas = root["capas"];
            int cantidad_de_capas = 0;

            for ( int index = 0; index < capas.size(); ++index ){
                
                string nombre_archivo;

                cantidad_de_capas++;

                if (!capas[index].isMember("imagen_fondo")){
                    logger->log_error("Esta capa no tiene el valor imagen_fondo, se cargará la capa por default");
                }
                
                try {
                    nombre_archivo = capas[index].get("imagen_fondo", IMAGEN_DEFAULT).asString(); // este default hay que ponerlo bien
                } catch(const runtime_error& error){
                    logger->log_error("Hay un número en el nombre de la capa");
                    nombre_archivo = "";

                }
                

                
                if (!exists_test(nombre_archivo)){
                    logger->log_error("La capa no fue encontrada, cargando capa por default");
                    nombre_archivo = IMAGEN_DEFAULT;
                }
                
                logger->log_debug("Intentando cargar capa '" + nombre_archivo + "'");

                float ancho_logico_capa = cargarValidar(capas[index], 700, "anchoLogico","Ancho lógico de capa no encontrado, se toma 700 por default");

                if (ancho_logico_capa < ventana_ancho){
                    logger->log_error("La capa no puede tener un ancho lógico menor menor al de la ventana, se usa como ancho de esta el ancho de la ventana");
                    ancho_logico_capa = ventana_ancho;

                }

                Capa *temp = new Capa(
                    nombre_archivo, ancho_logico_capa, DEFAULT_X_LOGICO, 
                    NULL,
                    NULL
                );

                capas_vector.push_back(temp);
            }

            if (cantidad_de_capas == 0){
                logger->log_error("No se encontró ninguna capa");
                logger->log_warning("Se carga una capa por default, con el ancho de la ventana/2");
                Capa *temp = new Capa(
                    IMAGEN_DEFAULT, ventana_ancho/2, DEFAULT_X_LOGICO, 
                    NULL,
                    NULL
                );
                capas_vector.push_back(temp);
            }

            personaje_mirar_derecha = cargarValidarBool(personaje, true, "mirar_derecha", "El personaje no tiene mirar_derecha, se carga por default derecha");

            personaje_zindex = cargarValidar(personaje, 0, "zindex", "Usando zindex (personaje) default de 0");

            if (personaje_zindex < 0){
                logger->log_error("Z-Index del personaje no puede ser negativo, usando Z maximo posible");
                personaje_zindex = capas.size();
            }

        } else {
            logger->log_error("Error de sytaxis en el archivo");
            logger->log_error(reader.getFormatedErrorMessages());
            cargarDefault();
        }
    }
}

float Conf::cargarValidar(Json::Value objetoJson, float valorDefault, char* clave, char* mensaje){
    float result = 0;
   if (!objetoJson.isMember(clave)){
        logger->log_warning(std::string("Json no tiene el parametro: ") + clave);
        logger->log_debug(mensaje);
    }
    
    try {
        result = objetoJson.get(clave, valorDefault).asFloat();
    }
    catch (const runtime_error& error){
        std::ostringstream buff;
        buff << valorDefault;
        logger->log_error("Se esperaba un número en" + std::string(clave) + ", se usá el valor por default" + buff.str());
        result = valorDefault;
    }
    return result;
}

bool Conf::cargarValidarBool(Json::Value objetoJson, bool valorDefault, char* clave, char* mensaje){
   if (!objetoJson.isMember(clave)){
        logger->log_warning(std::string("Json no tiene el parametro: ") + clave);
        logger->log_debug(mensaje);
    }
    return objetoJson.get(clave, valorDefault).asBool();
}


void Conf::cargarDefault(){
    logger = Logger::instance();
    logger->log_debug("Cargando configuración default");
    Conf::set_values("resources/default.json");
}
