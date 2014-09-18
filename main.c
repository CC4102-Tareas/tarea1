#include <stdio.h>
#include <stdlib.h>
#include "estructuras.c"
#include "dynamic_array.c"
#include "r-tree.c"
#include "operadores.c"

int main (int arc, char **argv) 
{
    printf("Tamaño de Nodo  -> %lu bytes\n", (unsigned long)sizeof(Nodo));
    
    printf("Inicio de experimento\n");   
	
    Nodo raiz;
    
    Punto x1;
    Punto x2;
    Rectangulo rect;
    Rectangulo rect0;

    int i;
    for(i=0;i<205;i++) {
        x1.x = 0;
        x1.y = 0;
        x2.x = i+1;
        x2.y = i+1;

        rect.vert_inf_izq = x1;
        rect.vert_sup_der = x2;
        
        if (i==0) {
            rect0.vert_inf_izq = x1;
            rect0.vert_sup_der = x2;
        } else if(i==1) {
            raiz = init_rtree(rect0, rect);
            printf("Inicialización de r-tree\n");
        } else {
            raiz = insertar(raiz, rect);
            printf("Se inserta rectangulo.\n");
        }
	}
    
    return 0; // todo ok.
}

