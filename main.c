#include <stdio.h>
#include <stdlib.h>
#include "estructuras.c"
#include "dynamic_array.c"
#include "r-tree.c"
#include "operadores.c"

/**
	TEST
*/
int main (int arc, char **argv) 
{
    /********************************************************
    Prueba para r-tree.c
    *********************************************************/
    /*
    Punto x1, x2;
    Rectangulo rect1, rect2;

    x1.x = 0;x1.y = 0;
    x2.x = 1;x2.y = 1;
    rect1.vert_inf_izq = x1;rect1.vert_sup_der = x2;

    x1.x = 1;x1.y = 1;
    x2.x = 2;x2.y = 2;
    rect2.vert_inf_izq = x1;rect2.vert_sup_der = x2;

    Nodo nodo = init_rtree(rect1, rect2);

    printf("nodo_id=%d|0\n", nodo.nodo_id);
    printf("nodo_padre=%d|0\n", nodo.nodo_padre);
    printf("pos_mbr_padre=%d|0\n", nodo.pos_mbr_padre);
    printf("nodo_ultimo=%d|1\n", nodo.ultimo);
    printf("nodo.mbr[0].nodo_hijo=%d|-1\n", nodo.mbr[0].nodo_hijo);    
    printf("nodo.mbr[1].nodo_hijo=%d|-1\n", nodo.mbr[1].nodo_hijo);    
    */
    /********************************************************
    Prueba para operadores.c
    *********************************************************/



    /********************************************************
    Prueba general
    *********************************************************/
    
    printf("Inicio de experimento\n");   
	Nodo raiz;
    
    Punto x1;
    Punto x2;
    Rectangulo rect;
    Rectangulo rect0;

    int i;
    for(i=0;i<10;i++) {
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

