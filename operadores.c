
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


/**
	busca un rectángulo dentro del R-Tree.
*/
Dynamic_array* buscar(Nodo nodo, Rectangulo rect) {
	int i, j;
	
	Dynamic_array *rects; // arreglo dinámico de rectangulos
	initArray(rects, 1);
				
	// recorremos los MBR's del nodo
	for(i=0;i<=nodo.ultimo;i++) {
		// si el rectangulo se intersecta con un MBR
		if (interseccion(rect, nodo.mbr[i].rect)){
			// si es una hoja. retornar rectangulo;
			if (nodo.mbr[i].nodo_hijo == -1) {
				insertArray(rects, nodo.mbr[i].rect);
			// seguir en profundidad
			} else {
				// buscar en los hijos
				Nodo nodo_hijo = leer_nodo(nodo.mbr[i].nodo_hijo);
				Dynamic_array *rects_hijos = buscar(nodo_hijo, rect);
				
				// se insertan en el grupo general
				for(j=0;j<rects_hijos->used;j++) {
					insertArray(rects, rects_hijos->array[j]);
				}
				
				// se libera la memoria.
				freeArray(rects); 	
			}	
		}
	}
	
	return rects;
}

/**
	Inserta un rectangulo en el arbol.
*/
Nodo insertar(Nodo nodo, Rectangulo rect) {
	int i, j;                     // iteradores
	int i_min = 0;     	          // indice del MBR con menor incremento de área.
	Rectangulo rect_area_inc_min; // rectángulo producto del incremento mínimo
	Rectangulo rect_area_inc;     // variable para albergar el incremento de área en cada iteración del for
	
	// si el nodo es un nodo hoja (su primer MBR es una hoja).
	// insertar rectangulo como un MBR
    // TODO: Verificar si está lleno y agregar split.

    //printf("nodo_id=%d\n", nodo.nodo_id);
    //printf("nodo_padre=%d\n", nodo.nodo_padre);
    //printf("pos_mbr_padre=%d\n", nodo.pos_mbr_padre);
    printf("nodo_ultimo=%d\n", nodo.ultimo);
    //printf("nodo_hijo=%d\n", nodo.mbr[0].nodo_hijo);
	if (nodo.mbr[0].nodo_hijo == -1) {
        printf("Insertar rectangulo.\n");
		
        // si está lleno
        if (nodo.ultimo == 2*T-1) {
            printf("Está lleno. Hacer split");
            //TODO: llamar algoritmo de split
        } else {
            MBR nueva_hoja;
		    nueva_hoja.rect = rect;
		    nueva_hoja.nodo_hijo = -1;
		
		    nodo.ultimo++;
		    nodo.mbr[nodo.ultimo].rect = rect;
		    nodo.mbr[nodo.ultimo].nodo_hijo = -1;
		
		    // persistir cambios en el archivo.
		    actualizar_nodo(nodo);
        }
	} else {
        printf("No es hoja. Buscar MBR de incremento mínimo.\n");
		rect_area_inc_min = incremento_area(nodo.mbr[i_min], rect);	

		// recorremos los MBR's del nodo e identificamos el de menor área.
		for(i=0;i<=nodo.ultimo;i++) {		
			// calcular el incremento de área para cada MBR
			rect_area_inc = incremento_area(nodo.mbr[i], rect);
			
			// se asume que el primer MBR es la mín área
			if (i > 0) {
				if (area(rect_area_inc) < area(rect_area_inc_min)) {
					i_min = i;
					rect_area_inc_min = incremento_area(nodo.mbr[i_min], rect);
				// si los incrementos son iguales => elegir el de menor área
				} else if (area(rect_area_inc) == area(rect_area_inc_min)) {				
					if (area(nodo.mbr[i].rect) < area(nodo.mbr[i_min].rect)) {
						i_min = i;
						rect_area_inc_min = incremento_area(nodo.mbr[i_min], rect);
					} 
					// si las areas son iguales nos quedamos con el primer área mínima que encontramos.
					// i.e. no se hace nada más
				}
			}
		}
	
		// persistir el incremento de área del MBR con indice i_min
		nodo.mbr[i_min].rect = rect_area_inc_min;
		actualizar_nodo(nodo);

		// buscar nodo hijo del MBR de incremento mínimo.
		Nodo nodo_hijo = leer_nodo(nodo.mbr[i_min].nodo_hijo);
		insertar(nodo_hijo, rect);
	}
    return nodo;
}

/*************************************************************************
	Algoritmos para insertar
**************************************************************************/
/*
void quadraticSplit(Nodo nodo, js)
{
	// El nodo tiene 2*T+1 MBR
	int i;
	int cantidad_pares = ((2*T+1)*(2*T))/2;
	
	// almacena las areas de cada par
	float tamano_area[cantidad_pares];
	
	// recorremos los MBR's del nodo
	for(i=0;i<(2*T+1);i++) 
	{
		
	}
   
}

void linearSplit()
{
	
}
*/

