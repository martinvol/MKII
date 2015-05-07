class Rectangulo{

	public:

		Rectangulo(float empieza_alto, float empieza_ancho, float porcentaje_alto, float porcentaje_ancho, bool ataque);
		
		float empieza_alto, empieza_ancho;
		float porcentaje_alto, porcentaje_ancho;
		bool ataque;
		// acá puede ir una función que devuelva el rectangulo en función del tamaño de personaje
		~Rectangulo();
};