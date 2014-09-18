
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

Rectangulo mbr_minimo(Rectangulo rect_mbr, Rectangulo rect) {
    Rectangulo mbr_min;

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
    return area(mbr_minimo(mbr.rect, rect)) - area(mbr.rect);
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
		if (interseccion(nodo.mbr[i].rect, rect)){
			// si es una hoja. retornar rectangulo;
			if (nodo.mbr[i].nodo_hijo == -1) {
				insertArray(rects, nodo.mbr[i].rect);
			// seguir en profundidad
			} else {
				// buscar en los hijos
				Nodo nodo_hijo = leer_nodo_en_disco(nodo.mbr[i].nodo_hijo);
				Dynamic_array *rects_hijos = buscar(nodo_hijo, rect);
				
				// se insertan en el grupo general
				for(j=0;j<rects_hijos->used;j++) {
					insertArray(rects, rects_hijos->array[j]);
				}
				
				// se libera la memoria.
				freeArray(rects_hijos);
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
    // esto siempre se puede hacer ya que el MBR tiene como mínimo T rectangulos.
	// insertar rectangulo como un MBR
	if (nodo.mbr[0].nodo_hijo == -1) {
        printf("Insertar rectangulo.\n");
		
        // si está lleno
        if (nodo.ultimo == 2*T-1) {
            printf("-> Está lleno!!. Hacer split.\n");
            // genera dos nodos uno con T+1 rectangulos y otro con T.
            // aquí rertornamos uno de ellos, el que mantiene el id del viejo.
            quadratic_split(nodo, rect);

            // rescatar ultima versión del nodo.
            nodo = leer_nodo_en_disco(nodo.nodo_id);
        // si tiene espacio, insertar un rectángulo.
        } else {
            MBR nueva_hoja;
		    nueva_hoja.rect = rect;
		    nueva_hoja.nodo_hijo = -1;
		    
		    nodo.ultimo++;
		    nodo.mbr[nodo.ultimo] = nueva_hoja;
		
	        // persistir cambios en el archivo.
    	    actualizar_nodo_en_disco(nodo);        
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
			// si los incrementos son iguales => elegir MBR de menor área.
            // esto se hace para que el tamaño total no crezca demasiado 
            // (hacemos crecer al más chico)
			} else if (area_inc == area_inc_min && area(nodo.mbr[i].rect) < area(nodo.mbr[i_min].rect)) {
				i_min = i; //no se modifica el área mínima porque son iguales.
				// si las areas son iguales nos quedamos con el primer área mínima que encontramos.
				// i.e. no se hace nada más
			}
		}
	    
		// persistir el incremento de área del MBR con indice i_min
		nodo.mbr[i_min].rect = mbr_minimo(nodo.mbr[i_min].rect, rect);
        
		actualizar_nodo_en_disco(nodo);
        
		// buscar nodo hijo del MBR de incremento mínimo.
		Nodo nodo_hijo = leer_nodo(nodo.mbr[i_min].nodo_hijo);
		insertar(nodo_hijo, rect);
	}
    return nodo;
}

/*************************************************************************
	Algoritmos para insertar
**************************************************************************/

/**
    Realiza la división de un nodo en dos en tiempo cuadratico.
*/
void quadratic_split(Nodo nodo, Rectangulo rect)
{
	// El nodo tiene 2*T rect + rect = 2*T + 1 rect
	int i, j;           // iteradores
    float area_max = 0; // almacena el incremento de área máximo.
    float area_tmp;     // area calculada en cada iteración
    int rect1, rect2;   // indices del mbr que forman el incremento máximo.

	// calculamos el área producida por cada par
    // esto es (N*(N-1))/2 con N=2*T+1
	for(i=0;i<(2*T);i++) {
		for(j=i+1;j<(2*T+1);j++) {
            // el rectangulo en la posición 2*T es rect. Caso especial.
            if (j == 2*T) {
                area_tmp = incremento_area_quadratic_split(nodo.mbr[i].rect, rect);
            } else {
                area_tmp = incremento_area_quadratic_split(nodo.mbr[i].rect, nodo.mbr[j].rect);
            }

            // se guardan los rectangulos que producen el incremento máximo.
            if (area_tmp > area_max) {
                area_max = area_tmp;
                rect1 = i;
                rect2 = j;
            }
        }
	}
   
    // aquí ya conocemos los rectangulos que generan el incremento de área máximo.
    // están en los indices rect1 y rect2

    Nodo nodo1, nodo2;

    nodo1.nodo_id = nodo.nodo_id;
    nodo1.nodo_padre = nodo.nodo_padre;
    nodo1.pos_mbr_padre = nodo.pos_mbr_padre;
    nodo1.ultimo = 0;
    nodo1.mbr[nodo1.ultimo] = nodo.mbr[rect1];

    //TODO: evaluar si el padre ya está lleno.
    //nodo2.nodo_id = ??;
    nodo2.nodo_padre = nodo.nodo_padre;
    nodo2.pos_mbr_padre = nodo.pos_mbr_padre;
    nodo2.ultimo = 0;
    if (rect == 2*T) {
        nodo2.mbr[nodo2.ultimo].rect = rect;
        nodo2.mbr[nodo2.ultimo].nodo_hijo = -1;
    } else {
        nodo2.mbr[nodo2.ultimo] = nodo.mbr[rect2];
    }

    // aquí ya tenemos los dos nuevos nodos con su rectangulo inicial.
    
    // iteramos sobre los elementos
    for(i=0;i<(2*T+1);i++) {
        // si debo procesar el rectangulo 2*T+1 y no fue asignado al inicio => lo proceso.
        if (i == 2*T && rect2 != 2*T) {
            
        } else if (i != rect1 && i != rect2) {

        }
    }
}

/**
    Función auxiliar. Calcula el incremento de área para quadratic_split
*/
float incremento_area_quadratic_split(Rectangulo rect1, Rectangulo rect2) {
    return area(mbr_minimo(rect1, rect2)) - (area(rect1) + area(rect2));
}

/*
void linear_split()
{
	
}
*/

