#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED
#include <string>

using namespace std;

class Logger{
	
	private:
		static Logger* myinstance;
		Logger();
		~Logger();
		bool debug, warning, error;
		ofstream* archivo;
		
    public:
		/* La instancia obtenida sólo puede destruirse con destroy_instance. */
        static Logger* instance();
        static void destroy_instance();
        
        void set_debug(bool nuevo_stado);
        void set_warning(bool nuevo_stado);
        void set_error(bool nuevo_stado);
        void log_debug(std::string const &message);
        void log_warning(std::string const &message);
        void log_error(std::string const &message);
};

#endif // LOGGER_H_INCLUDED
