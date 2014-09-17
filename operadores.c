
#define TRUE 1
#define FALSE 0

/**
	Procedimiento basado en http://stackoverflow.com/questions/13390333/two-rectangles-intersection	
	verifica si existe una intersección entre 2 rectángulos dados.
*/
int interseccion (Rectangulo r1, Rectangulo r2) 
{
	if ((r1.vert_sup_der.x < r2.vert_inf_izq.x || r2.vert_sup_der.x < r1.vert_inf_izq.x) ||
	    (r1.vert_sup_der.y < r2.vert_inf_izq.y || r2.vert_sup_der.y < r1.vert_inf_izq.y))
		return FALSE;
	
	return TRUE;
}

/**
	Entrega el área de un rectángulo.
*/
float area(Rectangulo rect) 
{
	float ancho = rect.vert_sup_der.x - rect.vert_inf_izq.x;
	float alto = rect.vert_sup_der.y - rect.vert_inf_izq.y;
	return ancho*alto;
}

/**
	Calcula el incremento de área de un mbr.
*/
Rectangulo incremento_area(MBR mbr, Rectangulo rect) {
	Rectangulo rect_inc;
	Punto pto1;
	Punto pto2;

	pto1.x = 0;
	pto1.y = 0;
	
	pto2.x = 1;
	pto2.y = 1;

	rect_inc.vert_inf_izq = pto1;
	rect_inc.vert_sup_der = pto2;

	return rect_inc;
}

