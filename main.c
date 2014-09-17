#include <stdio.h>
#include <stdlib.h>
#include "estructuras.c"
#include "dynamic_array.c"
#include "operadores.c"
#include "r-tree.c"

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
void insertar(Nodo nodo, Rectangulo rect) {
	int i, j;                     // iteradores
	int i_min = 0;     	      // indice del MBR con menor incremento de área.
	Rectangulo rect_area_inc_min; // rectángulo producto del incremento mínimo
	Rectangulo rect_area_inc;     // variable para albergar el incremento de área en cada iteración del for
	
	// si el nodo es un nodo hoja (su primer MBR es una hoja).
	// insertar rectangulo como un MBR
	if (nodo.mbr[0].nodo_hijo == -1) {
		MBR nueva_hoja;
		nueva_hoja.rect = rect;
		nueva_hoja.nodo_hijo = -1;
		
		nodo.ultimo++;
		nodo.mbr[nodo.ultimo].rect = rect;
		nodo.mbr[nodo.ultimo].nodo_hijo = -1;
		
		// persistir cambios en el archivo.
		actualizar_nodo(nodo);
	} else {

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
}

/*************************************************************************
	Algoritmos para insertar
**************************************************************************/
/*
void quadraticSplit(Nodo nodo, js)
{
	// El nodo tiene 2*T+1 MBR
	int i;
	int cantidad_pares = ((2*T+1)(2*T))/2;
	
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
/**
	===================================================================
	MAIN
	===================================================================
	
	Para compilar: gcc -o tarea1-CC4102 rectangulo.c
	Para ejecutar: ./tarea1-CC4102
*/
int main (int arc, char **argv) 
{   
	// se inicializa el r-tree
	Nodo raiz = init_rtree();
    
    Punto x1;
    Punto x2;
    Rectangulo rect;

    int i;
    for(i=0;i<10;i++) {
        x1.x = 0;
        x1.y = 0;
        x2.x = 3;
        x2.y = 3;

        rect.vert_inf_izq = x1;
        rect.vert_sup_der = x2;

        insertar_nodo(raiz, rect);
	}

    return 0; // todo ok.
}

