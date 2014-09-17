#include <stdio.h>
#include <stdlib.h>
#include "estructuras.c"
#include "dynamic_array.c"
#include "r-tree.c"
#include "operadores.c"

/**
	MAIN
*/
int main (int arc, char **argv) 
{
    printf("Inicio de experimento\n");   
	// se inicializa el r-tree
	Nodo raiz = init_rtree();
    printf("ok0\n");
    Punto x1;
    Punto x2;
    Rectangulo rect;
    printf("ok1\n");
    int i;
    for(i=0;i<10;i++) {
        x1.x = 0;
        x1.y = 0;
        x2.x = 3;
        x2.y = 3;

        rect.vert_inf_izq = x1;
        rect.vert_sup_der = x2;
        printf("ok2\n");
        insertar(raiz, rect);
        printf("ok3\n");
	}
    
    return 0; // todo ok.
}

