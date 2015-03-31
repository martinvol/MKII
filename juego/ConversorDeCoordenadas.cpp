ConversorDeCoordenadas::ConversorDeCoordenadas(float f_ancho, float f_alto, float x, float y){
	this->x_logico = x;
	this->y_logico = y;
	this->factor_ancho = f_ancho;
	this->factor_alto = f_alto;
}

CoordenadasFisicas* ConversorDeCoordenadas::convertirAFisicas(CoordenadasLogicas coord){
	return new CoordenadasFisicas((coord.coord_x-x_logico)*f_x, (coord.coord_y-y_logico)*f_y);
}

CoordenadasLogicas* ConversorDeCoordenadas::convertirALogicas(CoordenadasFisicas coord){
	return new CoordenadasFisicas(x_logico + coord.x_fisico * 1.0 / factor_ancho, y_logico + coord.y_fisico * 1.0 / factor_alto);
}

void ConversorDeCoordenadas::seMovioVentana(int pixeles){
	x_logico += pixeles * 1.0 / factor_ancho;
}

void ConversorDeCoordenadas::seMovioVentana(float metros){
	x_logico += metros;
}
