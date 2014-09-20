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
void insertar(Nodo nodo, Rectangulo rect) {
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
            Resultado_split nuevos_nodos = quadratic_split(nodo, rect);
            
            actualizar_nodo_en_disco(nuevos_nodos.nodo1);

            nuevos_nodos.nodo2

            // si es la raiz
            if (nodo.nodo_padre == -1) {
                Nodo raiz;

                ultimo_id_utilizado++; // variable global creada en el main para el .
                raiz.nodo_id = nuevo_id;
                raiz.nodo_padre = -1;
                raiz.pos_mbr_padre = -1;
                raiz.ultimo = 1;

                raiz.mbr[0].rect = nuevos_nodos.mbr1;
                raiz.mbr[0].nodo_hijo = nuevos_nodos.nodo1.nodo_id;
                raiz.mbr[1].rect = nuevos_nodos.mbr2;
                raiz.mbr[1].nodo_hijo = nuevos_nodos.nodo2.nodo_id;
                
                insertar_nodo_en_disco(raiz);
            } else {                
                Nodo nodo_padre = leer_nodo_en_disco(nodo.nodo_padre);
            
            
                insertar_nodo_en_disco(nuevos_nodos.nodo2);

                // si el padre está lleno
                if (nodo_padre.ultimo == 2*T-1) {

                    quadratic_split(nodo_padre, nuevos_nodos.mbr2);
                // entonces hay espacio. Puede ser un piso intermedio.
                } else {
                    // se agrega el nuevo nodo al padre.
                    nodo_padre.ultimo++;
                    nodo_padre.mbr[nodo_padre.ultimo].rect = nuevos_nodos.mbr2;
                    nodo_padre.mbr[nodo_padre.ultimo].nodo_hijo = nuevos_nodos.nodo2.nodo_id;
                    
                    // se actualiza el mbr del nodo padre de ambos.
                    nodo_padre.mbr[nuevos_nodos.nodo1.pos_mbr_padre].rect = nuevos_nodos.mbr1;
                    
                    // actualizamos la referencia al padre de nodo1.
                    nuevos_nodos.nodo1.nodo_padre = nodo.nodo_padre;
                    nuevos_nodos.nodo1.pos_mbr_padre = nodo.pos_mbr__padre;

                    // actualizamos la referencia al padre de nodo2.
                    nuevos_nodos.nodo2.nodo_padre = nodo_padre.nodo_id;
                    nuevos_nodos.nodo2.pos_mbr_padre = nodo_padre.ultimo;

                    actualizar_nodo_en_disco(nuevos_nodos.nodo1);
                    actualizar_nodo_en_disco(nodo_padre);
                    insertar_nodo_en_disco(nuevos_nodos.nodo2);

                    // se actualizan las referencias de los nodos hijos.
                    Nodo nodo_aux;
                    for(i=0;nuevos_nodos.nodo1.ultimo;i++) {
                         nodo_aux = leer_nodo_en_disco(nuevos_nodos.nodo1.mbr[i].nodo_hijo);
                         nodo_aux.pos_mbr_padre = i;
                         actualizar_nodo_en_disco(nodo_aux);
                    }

                    for(i=0;nuevos_nodos.nodo2.ultimo;i++) {
                         nodo_aux = leer_nodo_en_disco(nuevos_nodos.nodo1.mbr[i].nodo_hijo);
                         nodo_aux.pos_mbr_padre = i;
                         actualizar_nodo_en_disco(nodo_aux);
                    }
                }
            }               
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
		Nodo nodo_hijo = leer_nodo_en_disco(nodo.mbr[i_min].nodo_hijo);
		insertar(nodo_hijo, rect);
	}
}

/*************************************************************************
	Algoritmos para insertar
**************************************************************************/

/**
    Función auxiliar. Calcula el incremento de área para split
*/
float incremento_area_split(Rectangulo rect1, Rectangulo rect2) {
    return area(mbr_minimo(rect1, rect2)) - (area(rect1) + area(rect2));
}

/**
    Realiza la división de un nodo en dos en tiempo cuadratico.
*/
Resultado_split quadratic_split(Nodo nodo, Rectangulo rect)
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
                area_tmp = incremento_area_split(nodo.mbr[i].rect, rect);
            } else {
                area_tmp = incremento_area_split(nodo.mbr[i].rect, nodo.mbr[j].rect);
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

    // nodo1.nodo_id = -> se asigna en insert
    // nodo1.nodo_padre = -> se asigna en insert
    // nodo1.pos_mbr_padre = -> se asigna en insert
    nodo1.ultimo = 0;
    nodo1.mbr[0] = nodo.mbr[rect1];

    // nodo2.nodo_id = -> se asigna en insert
    // nodo2.nodo_padre = -> se asigna en insert;
    // nodo2.pos_mbr_padre = -> se asigna en insert;
    nodo2.ultimo = 0;
    if (rect2 == 2*T) {
        nodo2.mbr[0].rect = rect;
        nodo2.mbr[0].nodo_hijo = -1;
    } else {
        nodo2.mbr[0] = nodo.mbr[rect2];
    }
    
    // mantener un rectangulo que representa el mbr que va en el padre para cada nodo.
    // esto es util para ir calculando el incremento de área en la iteración.
    Rectangulo mbr1, mbr2;
    float area_inc1, area_inc2;

    // en un principio será = al rectangulo inicial.
    mbr1 = nodo1.mbr[nodo1.ultimo].rect;
    mbr2 = nodo2.mbr[nodo2.ultimo].rect;

    // aquí ya tenemos los dos nuevos nodos con su rectangulo inicial. Además tenemos el mbr
    // de cada grupo.
    
    // iteramos sobre los elementos
    for(i=0;i<(2*T+1);i++) {
        // se saltan los escogidos al inicio.
        if (i != rect1 && i != rect2) {
            // si alguno completó su minimo     
            if (nodo1.ultimo == T+1) {
                nodo2.ultimo++;
                if (i==2*T) {           
                    nodo2.mbr[nodo2.ultimo].rect = rect;
                    nodo2.mbr[nodo2.ultimo].nodo_hijo = -1;
                    mbr2 = mbr_minimo(mbr2, rect);
                } else {
                    nodo2.mbr[nodo2.ultimo] = nodo.mbr[i];
                    mbr2 = mbr_minimo(mbr2, nodo.mbr[i].rect);
                }
                continue;
            } else if (nodo2.ultimo == T+1) {
                nodo1.ultimo++;
                if (i==2*T) {           
                    nodo1.mbr[nodo1.ultimo].rect = rect;
                    nodo1.mbr[nodo1.ultimo].nodo_hijo = -1;
                    mbr1 = mbr_minimo(mbr1, rect);
                } else {
                    nodo1.mbr[nodo1.ultimo] = nodo.mbr[i];
                    mbr1 = mbr_minimo(mbr1, nodo.mbr[i].rect);
                }
                continue;
            }

            // si debo procesar el rectangulo 2*T+1 dado que no fue asignado al inicio => lo proceso.
            if (i == 2*T) {
                area_inc1 = incremento_area_split(mbr1, rect);
                area_inc2 = incremento_area_split(mbr2, rect);
            } else {
                area_inc1 = incremento_area_split(mbr1, nodo.mbr[i].rect);
                area_inc2 = incremento_area_split(mbr2, nodo.mbr[i].rect);
            }
        
            //TODO: verificar si falta una condición aquí.
            if (area_inc1 < area_inc2) {
                nodo1.ultimo++;
                if (i==2*T) {           
                    nodo1.mbr[nodo1.ultimo].rect = rect;
                    nodo1.mbr[nodo1.ultimo].nodo_hijo = -1;
                    mbr1 = mbr_minimo(mbr1, rect);
                } else {
                    nodo1.mbr[nodo1.ultimo] = nodo.mbr[i];
                    mbr1 = mbr_minimo(mbr1, nodo.mbr[i].rect);
                }
            } else {
                nodo2.ultimo++;
                if (i==2*T) {
                    nodo2.mbr[nodo2.ultimo].rect = rect;
                    nodo2.mbr[nodo2.ultimo].nodo_hijo = -1;
                    mbr2 = mbr_minimo(mbr2, rect);
                } else {
                    nodo2.mbr[nodo2.ultimo] = nodo.mbr[i];
                    mbr2 = mbr_minimo(mbr2, nodo.mbr[i].rect);
                }
            }
        }
    }
    
    // en este punto los dos nodos tienen todos los rectangulos asignados.
    // uno con T+1 y el otro con T.
    
    Resultado_split resp;
    resp.nodo1 = nodo1;
    resp.nodo2 = nodo2;
    resp.mbr1 = mbr1;
    resp.mbr2 = mbr2;

    return resp;
}


void linear_split(Nodo nodo, Rectangulo rect) {
	
    int i; // itearador
    // se asume que el primer indice es el de menor y mayor distancia
    // en ambas dimensiones.
    int id_min_lado_mayor_x=0, id_max_lado_menor_x=0;
    int id_min_lado_mayor_y=0, id_max_lado_menor_y=0;
    
    float max_lado_mayor_x, max_lado_menor_y;

    float min_lado_mayor_x, max_lado_menor_x;
    float min_lado_mayor_y, max_lado_menor_y;

    float w_x, w_y;

    // indices de los rectángulos R1 y R2
    int rect1, rect2;
    // mbr que contiene a cada uno de los nodos nuevos
    Rectangulo mbr1, mbr2;

    for(i=1;i<=(2*T+1);i++) {
        // ocupar rect
        if (i==2*T) {
            // buscamos el lado mayor mínimo en el eje X.
            if (rect.vert_sup_der.x < nodo.mbr[id_min_lado_mayor_x].rect.vert_sup_der.x) {
                id_min_lado_mayor_x = i;
                min_lado_mayor_x = rect.vert_sup_der.x
            } 
            // buscamos el lado menor maximo en el eje x
            if (rect.vert_inf_izq.x > nodo.mbr[id_max_lado_menor_x].rect.vert_inf_izq.x) {
                id_max_lado_menor_x = i;
                max_lado_menor_x = rect.vert_inf_izq.x;
            }
            // buscamos el lado mayor mínimo en el eje Y.
            if (rect.vert_sup_der.y < nodo.mbr[id_min_lado_mayor_y].rect.vert_sup_der.y) {
                id_min_lado_mayor_y = i;
                min_lado_mayor_y = rect.vert_sup_der.y;
            } 
            // buscamos el lado menor maximo en el eje Y
            if (rect.vert_inf_izq.y > nodo.mbr[id_max_lado_menor_y].rect.vert_inf_izq.y) {
                id_max_lado_menor_y = i;
                max_lado_menor_y = rec.vert_inf_izq.y
            }
            // buscamos el lado mayor maximo en el eje Y
            if (rect.vert_sup_der.y > max_lado_mayor_y) {
                max_lado_mayor_y = rec.vert_sup_der.y;
            }
            // buscamos el lado mayor maximo en el eje X
            if (rect.vert_sup_der.x > max_lado_mayor_x) {
                max_lado_mayor_x = rec.vert_sup_der.x;
            }
        } else {
            // buscamos el lado mayor mínimo en el eje X.
            if (nodo.mbr[i].rect.vert_sup_der.x < nodo.mbr[id_min_lado_mayor_x].rect.vert_sup_der.x) {
                id_min_lado_mayor_x = i;
                min_lado_mayor_x = nodo.mbr[i].rect.vert_sup_der.x;
            } 
            // buscamos el lado menor maximo en el eje x
            if (nodo.mbr[i].rect.vert_inf_izq.x > nodo.mbr[id_max_lado_menor_x].rect.vert_inf_izq.x) {
                id_max_lado_menor_x = i;
                max_lado_menor_x = nodo.mbr[i].rect.vert_inf_izq.x;
            }
            // buscamos el lado mayor mínimo en el eje Y.
            if (nodo.mbr[i].rect.vert_sup_der.y < nodo.mbr[id_min_lado_mayor_y].rect.vert_sup_der.y) {
                id_min_lado_mayor_y = i;
                min_lado_mayor_y = nodo.mbr[i].rect.vert_sup_der.y;
            } 
            // buscamos el lado menor maximo en el eje Y
            if (nodo.mbr[i].rect.vert_inf_izq.y > nodo.mbr[id_max_lado_menor_y].rect.vert_inf_izq.y) {
                id_max_lado_menor_y = i;
                max_lado_menor_y = nodo.mbr[i].rect.vert_inf_izq.y;
            }
            // buscamos el lado mayor maximo en el eje Y
            if (nodo.mbr[i].rect.vert_sup_der.y > max_lado_mayor_y) {
                max_lado_mayor_y = nodo.mbr[i].rec.vert_sup_der.y;
            }
            // buscamos el lado mayor maximo en el eje X
            if (nodo.mbr[i].rect.vert_sup_der.x > max_lado_mayor_x) {
                max_lado_mayor_x = nodo.mbr[i].rec.vert_sup_der.x;
            }
        }
    }

    // aquí tenemos todos los rectángulos. Calculamos w_x y w_y
    w_x = (max_lado_menor_x - min_lado_mayor_x)/max_lado_mayor_x;
    w_y = (max_lado_menor_y - min_lado_mayor_y)/max_lado_mayor_y;

    if(w_x > w_y) {
        rect1 =;
        rect2 =;
    } else {
        rect1 =;
        rect2 =;
    }

    // aquí ya conocemos los rectangulos que generan el incremento de área máximo.
    // están en los indices rect1 y rect2

    Nodo nodo1, nodo2;

    nodo1.nodo_id = nodo.nodo_id;
    nodo1.nodo_padre = nodo.nodo_padre;
    nodo1.pos_mbr_padre = nodo.pos_mbr_padre;
    nodo1.ultimo = 0;
    if (rect1 == 2*T) {
        nodo1.mbr[0].rect = rect;
        nodo1.mbr[0].nodo_hijo = -1;
    } else {
        nodo1.mbr[0] = nodo.mbr[rect1];
    }

    nodo2.nodo_id = -1;
    // al final se evalua si continua con el padre
    nodo2.nodo_padre = nodo.nodo_padre;
    nodo2.pos_mbr_padre = nodo.pos_mbr_padre;
    nodo2.ultimo = 0;
    if (rect2 == 2*T) {
        nodo2.mbr[0].rect = rect;
        nodo2.mbr[0].nodo_hijo = -1;
    } else {
        nodo2.mbr[0] = nodo.mbr[rect2];
    }
    
    // mantener un rectangulo que representa el mbr que va en el padre para cada nodo.

    // en un principio será = al rectangulo inicial.
    mbr1 = nodo1.mbr[nodo1.ultimo].rect;
    mbr2 = nodo2.mbr[nodo2.ultimo].rect;

    // aquí ya tenemos los dos nuevos nodos con su rectangulo inicial. Además tenemos el mbr
    // de cada grupo.
    
    // iteramos sobre los elementos
    for(i=0;i<(2*T+1);i++) {
        // se saltan los escogidos al inicio.
        if (i != rect1 && i != rect2) {
            // si alguno completó su minimo
            if (nodo1.ultimo == T+1) {
                nodo2.ultimo++;
                if (i==2*T) {           
                    nodo2.mbr[nodo2.ultimo].rect = rect;
                    nodo2.mbr[nodo2.ultimo].nodo_hijo = -1;
                    mbr2 = mbr_minimo(mbr2, rect);
                } else {
                    nodo2.mbr[nodo2.ultimo] = nodo.mbr[i];
                    mbr2 = mbr_minimo(mbr2, nodo.mbr[i].rect);
                }
                continue;
            } else if (nodo2.ultimo == T+1) {
                nodo1.ultimo++;
                if (i==2*T) {           
                    nodo1.mbr[nodo1.ultimo].rect = rect;
                    nodo1.mbr[nodo1.ultimo].nodo_hijo = -1;
                    mbr1 = mbr_minimo(mbr1, rect);
                } else {
                    nodo1.mbr[nodo1.ultimo] = nodo.mbr[i];
                    mbr1 = mbr_minimo(mbr1, nodo.mbr[i].rect);
                }
                continue;
            }

            // si debo procesar el rectangulo 2*T+1 dado que no fue asignado al inicio => lo proceso.
            if (i == 2*T) {
                area_inc1 = incremento_area_split(mbr1, rect);
                area_inc2 = incremento_area_split(mbr2, rect);
            } else {
                area_inc1 = incremento_area_split(mbr1, nodo.mbr[i].rect);
                area_inc2 = incremento_area_split(mbr2, nodo.mbr[i].rect);
            }
        
            //TODO: verificar si falta una condición aquí.
            if (area_inc1 < area_inc2) {
                nodo1.ultimo++;
                if (i==2*T) {           
                    nodo1.mbr[nodo1.ultimo].rect = rect;
                    nodo1.mbr[nodo1.ultimo].nodo_hijo = -1;
                    mbr1 = mbr_minimo(mbr1, rect);
                } else {
                    nodo1.mbr[nodo1.ultimo] = nodo.mbr[i];
                    mbr1 = mbr_minimo(mbr1, nodo.mbr[i].rect);
                }
            }else if (area_inc1 > area_inc2) {
                nodo2.ultimo++;
                if (i==2*T) {
                    nodo2.mbr[nodo2.ultimo].rect = rect;
                    nodo2.mbr[nodo2.ultimo].nodo_hijo = -1;
                    mbr2 = mbr_minimo(mbr2, rect);
                } else {
                    nodo2.mbr[nodo2.ultimo] = nodo.mbr[i];
                    mbr2 = mbr_minimo(mbr2, nodo.mbr[i].rect);
                }
            // son iguales. Se lleva al grupo con menor área.
            } else if (area(mbr1) < area(mbr2)) {
                nodo1.ultimo++;
                if (i==2*T) {           
                    nodo1.mbr[nodo1.ultimo].rect = rect;
                    nodo1.mbr[nodo1.ultimo].nodo_hijo = -1;
                    mbr1 = mbr_minimo(mbr1, rect);
                } else {
                    nodo1.mbr[nodo1.ultimo] = nodo.mbr[i];
                    mbr1 = mbr_minimo(mbr1, nodo.mbr[i].rect);
                }
            } else if (area(mbr1) > area(mbr2)) {
                nodo2.ultimo++;
                if (i==2*T) {
                    nodo2.mbr[nodo2.ultimo].rect = rect;
                    nodo2.mbr[nodo2.ultimo].nodo_hijo = -1;
                    mbr2 = mbr_minimo(mbr2, rect);
                } else {
                    nodo2.mbr[nodo2.ultimo] = nodo.mbr[i];
                    mbr2 = mbr_minimo(mbr2, nodo.mbr[i].rect);
                }
            // son iguales. Se lleva al con menor n° de rectángulos.
            } else if(nodo1.ultimo < nodo2.ultimo) {
                nodo1.ultimo++;
                if (i==2*T) {           
                    nodo1.mbr[nodo1.ultimo].rect = rect;
                    nodo1.mbr[nodo1.ultimo].nodo_hijo = -1;
                    mbr1 = mbr_minimo(mbr1, rect);
                } else {
                    nodo1.mbr[nodo1.ultimo] = nodo.mbr[i];
                    mbr1 = mbr_minimo(mbr1, nodo.mbr[i].rect);
                }
            } else if(nodo1.ultimo > nodo2.ultimo) { 
                nodo2.ultimo++;
                if (i==2*T) {
                    nodo2.mbr[nodo2.ultimo].rect = rect;
                    nodo2.mbr[nodo2.ultimo].nodo_hijo = -1;
                    mbr2 = mbr_minimo(mbr2, rect);
                } else {
                    nodo2.mbr[nodo2.ultimo] = nodo.mbr[i];
                    mbr2 = mbr_minimo(mbr2, nodo.mbr[i].rect);
                }
            // decisión arbitraria. Insertar en primer nodo
            } else {
                nodo1.ultimo++;
                if (i==2*T) {           
                    nodo1.mbr[nodo1.ultimo].rect = rect;
                    nodo1.mbr[nodo1.ultimo].nodo_hijo = -1;
                    mbr1 = mbr_minimo(mbr1, rect);
                } else {
                    nodo1.mbr[nodo1.ultimo] = nodo.mbr[i];
                    mbr1 = mbr_minimo(mbr1, nodo.mbr[i].rect);
                }
            }
        }
    }

    // en este punto los dos nodos tienen todos los rectangulos asignados.
    // uno con T+1 y el otro con T.

    actualizar_nodo_en_disco(nodo1);

    Nodo nodo_padre = leer_nodo_en_disco(nodo.nodo_padre);

    // se modifica el nodo viejo que se dividió.
    // el identificador(nodo_hijo) sigue siendo el mismo.
    nodo_padre.mbr[nodo.pos_mbr_padre].rect = mbr1;

    // si colapsa la raiz
    if (nodo.nodo_id == nodo.nodo_padre) {
        nodo raiz;
        raiz.nodo_id =;
        raiz.nodo_padre = -1;

    // si colapsa un nodo intermedio
    } else if (nodo_padre.ultimo == 2*T) {        
        linear_split(nodo_padre, mbr2);
    } else {
        // se agrega el nuevo nodo.
        nodo_padre.ultimo++;
        nodo_padre.mbr[nodo_padre.ultimo].rect = mbr2;
        nodo_padre.nodo_hijo = nodo2.nodo_id;

        actualizar_nodo_en_disco(nodo_padre);
    }

}


