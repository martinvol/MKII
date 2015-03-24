#include <cstdio>
#include <cstring>
#include <fstream>

// This is the JSON header
#include "jsoncpp/json/json.h"

using namespace std;


class Conf {

    public:
        void set_values (char* file_name);
        int ventana_anchopx, ventana_altopx, ventana_ancho;
        int escenario_ancho, escenario_alto, escenario_ypiso;
        bool valido = false; // Si este valor es falso el archivo se cargó mal
};


void Conf::set_values (char* file_name) {
    Json::Value root;
    Json::Reader reader;

    if (file_name == NULL){
            puts("No hay archivo");
            return;
    }
    
    std::ifstream test(file_name, std::ifstream::binary);
    bool parsingSuccessful = reader.parse(test, root, false);

    if (parsingSuccessful){

        const Json::Value ventana = root["ventana"];
        
        ventana_anchopx = ventana.get("anchopx", 0).asInt();
        ventana_altopx = ventana.get("altopx", 0).asInt();
        ventana_ancho = ventana.get("ancho", 0).asInt();

        const Json::Value escenario = root["escenario"];
        escenario_ancho = escenario.get("ancho", 0).asInt();
        escenario_alto = escenario.get("alto", 0).asInt();
        escenario_ypiso = escenario.get("ypiso", 0).asInt();

    } else {
        puts("Problema con el archivo, probablemente no existe");
    }
}


int main(int argc, char* argv[]) {
    Conf conf;
    conf.set_values(argv[1]);
    printf ("conf.ventana_anchopx: %d \n", conf.ventana_anchopx);
    printf ("conf.ventana_altopx: %d \n", conf.ventana_altopx);
    printf ("conf.ventana_ancho: %d \n", conf.ventana_ancho);

    printf ("conf.escenario_ancho: %d \n", conf.escenario_ancho);
    printf ("conf.escenario_alto: %d \n", conf.escenario_alto);
    printf ("conf.escenario_ypiso: %d \n", conf.escenario_ypiso);

    return 0;
}