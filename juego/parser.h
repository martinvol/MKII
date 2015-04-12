#include <unordered_map>
#include "Capa.h"
#include "jsoncpp/json/json.h"
#include "logger.h"

class Conf {
	private:
		float cargarValidar(Json::Value, float, char*, char*);
        bool cargarValidarBool(Json::Value, bool, char*, char*);
		Logger *logger;

    public:
        void set_values (char* file_name);
        void cargarDefault();
        float ventana_anchopx, ventana_altopx, ventana_ancho;
        float escenario_ancho, escenario_alto, escenario_ypiso;
        float personaje_ancho, personaje_alto, personaje_zindex;
        float margen;
        std::vector<Capa*> capas_vector;
        unordered_map <string, string> sprites_map;

        bool personaje_mirar_derecha;
};