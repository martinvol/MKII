#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <unordered_map>
#include "jsoncpp/json/json.h"

#include "../Escenario/Capa.hpp"
#include "../Logger/Logger.hpp"

class Parser {
	private:
		float cargarValidar(Json::Value, float, char*, char*);
        bool cargarValidarBool(Json::Value, bool, char*, char*);
		Logger *logger;

    public:
		// El constructor no lo hacemos porque se supone que siempre después de
		// crearlo se llama a set_values inmediatamente, antes de llamar a
		// cualquier otra función.
		~Parser();

        void set_values (char* file_name);
        void cargarDefault();
        float ventana_anchopx, ventana_altopx, ventana_ancho;
        float escenario_ancho, escenario_alto, escenario_ypiso;
        float personaje_ancho, personaje_alto, personaje_zindex;
        float margen;
        std::vector<Capa*> capas_vector;
        unordered_map <string, string> sprites_map;
        bool personaje_mirar_derecha;
        bool valido = false; // Si este valor es falso el archivo se cargó mal
};

#endif // PARSER_H_INCLUDED
