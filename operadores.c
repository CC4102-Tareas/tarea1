
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
    Calcula el mínimo entre dos valores. Función auxiliar
*/
float min(float x, float y) {
    if (x > y)
        return y;
    return x;
}

/**
    Calcula el máximo entre dos valores. Función auxiliar
*/
float max(float x, float y) {
    if (x > y)
        return x;
    return y;
}

Rectangulo mbr_minimo(MBR mbr, Rectangulo rect) {
    Rectangulo rect_mbr, mbr_min;

    rect_mbr = mbr.rect;

    Punto pto1,pto2;

    pto1.x=min(rect_mbr.vert_inf_izq.x, rect.vert_inf_izq.x);
    pto1.y=min(rect_mbr.vert_inf_izq.y, rect.vert_inf_izq.y);
    pto2.x=max(rect_mbr.vert_sup_der.x, rect.vert_sup_der.x);
    pto2.y=max(rect_mbr.vert_sup_der.y, rect.vert_sup_der.y);
    
    mbr_min.vert_inf_izq = pto1;
    mbr_min.vert_sup_der = pto2;

    return mbr_min;
}

/**
	Calcula el incremento de área de un mbr.
*/
float incremento_area(MBR mbr, Rectangulo rect) {
    return area(mbr_minimo(mbr, rect)) - area(mbr.rect);
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
	int i, j;           // iteradores
	int i_min = 0;     	// indice del MBR con menor incremento de área.
	float area_inc_min; // valor del incremento mínimo de área.
	float area_inc;     // variable para albergar el incremento de área en cada iteración del for
	
	// si el nodo es un nodo hoja (su primer MBR es una hoja).
	// insertar rectangulo como un MBR
    // TODO: Verificar si está lleno y agregar split.

    printf("nodo_ultimo=%d\n", nodo.ultimo);
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
        // se asume que el primer MBR tiene el área mínima.
		area_inc_min = incremento_area(nodo.mbr[i_min], rect);	

		// recorremos los MBR's del nodo e identificamos el de menor área.
		for(i=1;i<=nodo.ultimo;i++) {		
			// calcular el incremento de área
		    area_inc = incremento_area(nodo.mbr[i], rect);
			
			if (area_inc < area_inc_min) {
				i_min = i;
				area_inc_min = area_inc;
			// si los incrementos son iguales => elegir MBR de menor área
			} else if (area_inc == area_inc_min) {				
				if (area(nodo.mbr[i].rect) < area(nodo.mbr[i_min].rect)) {
					i_min = i;
					area_inc_min = incremento_area(nodo.mbr[i_min], rect);
				} 
				// si las areas son iguales nos quedamos con el primer área mínima que encontramos.
				// i.e. no se hace nada más
			}
		}
	
		// persistir el incremento de área del MBR con indice i_min
		nodo.mbr[i_min].rect = mbr_minimo(nodo.mbr[i_min], rect);
        
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

