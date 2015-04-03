#include <cstdio>
#include <cstring>
#include <fstream>
#include <unordered_map>
#include "Capa.h"
#include "parser.h"
#include "logger.h"
// This is the JSON header
#include "jsoncpp/json/json.h"

using namespace std;


void Conf::set_values (char* my_file) {
    Logger *logger = Logger::instance();
    logger->log_debug("Inicializando parser");

    logger->log_debug(std::string("Intenta cargar configuraciones desde ") +  my_file);

    Json::Value root;
    Json::Reader reader;

    if (my_file == NULL){
            puts("No hay archivo");
            return;
    }
    
    std::ifstream test(my_file, std::ifstream::binary);

    if (!test.is_open()){
        logger->log_error("Problema con el archivo, probablemente no existe");
        Conf::set_values("pruebas/json/test.json");

    }

    else {

        bool parsingSuccessful = reader.parse(test, root, false);

        if (parsingSuccessful){

            const Json::Value ventana = root["ventana"];
            
            ventana_anchopx = ventana.get("anchopx", 0).asFloat();
            ventana_altopx = ventana.get("altopx", 0).asFloat();
            ventana_ancho = ventana.get("ancho", 0).asFloat();
            margen = ventana.get("margen", 0).asFloat();

            const Json::Value escenario = root["escenario"];
            escenario_ancho = escenario.get("ancho", 0).asFloat();
            escenario_alto = escenario.get("alto", 0).asFloat();
            escenario_ypiso = escenario.get("ypiso", 0).asFloat();


            const Json::Value personaje = root["personaje"];
            personaje_ancho = personaje.get("ancho", 0).asFloat();
            personaje_alto = personaje.get("alto", 0).asFloat();
            personaje_zindex = personaje.get("zindex", 0).asFloat();

            const Json::Value sprites = personaje["sprites"];

            for (auto const& id : sprites.getMemberNames()) {
                //std::cout << id << std::endl;
                sprites_map[id] = sprites.get(id, "Esto nunca se va a mostrar").asString();
            }

            const Json::Value capas = root["capas"];
            
            for ( int index = 0; index < capas.size(); ++index ){
    /*            std::cout << capas[index].get("imagen_fondo", "default").asString();
                std::cout << "\n";
                printf("capa: %f \n", capas[index].get("ancho", 0).asFloat());
    */
                Capa *temp = new Capa(
                    capas[index].get("imagen_fondo", "default").asString(), 
                    capas[index].get("anchoLogico", 0).asFloat(),
                    capas[index].get("xlogico", 0).asFloat(), 
                    NULL);

                capas_vector.push_back(temp);
            }

       
            valido = true;
        } else {
            logger->log_error("Error de sytaxis en el archivo");
            logger->log_error(reader.getFormatedErrorMessages());
            Conf::set_values("pruebas/json/test.json");

        }
    }
}