
#define TAMANO_PAGINA 4096 // tamaño de página del disco duro.
#define T 123              // mínimo de hijos que puede tener un nodo (salvo la raíz)

/**
	Definición de punto (x,y)
*/
typedef struct
{
	float x;
	float y;
} Punto;

/**
	--------------*
	|             |
	| Rectángulo  |
	|             |
	*--------------

	Asterisco representa los vértices.
*/
typedef struct
{
	Punto vert_inf_izq;
	Punto vert_sup_der;
} Rectangulo;

/**
	Minimun Bounding Rectangule
*/
typedef struct 
{
	Rectangulo rect; // MBR
	int nodo_hijo;   // posición del nodo en el archivo.
} MBR;

/**
	Nodo del R-Tree.
*/
typedef struct 
{
	int nodo_padre;    // posición del nodo padre en el archivo.
	int pos_mbr_padre; // posición del MBR padre en el nodo padre(índice del arreglo MBR).
	int ultimo;        // último indice ocupado en mbr.
	MBR mbr[2*T];      // arreglo de MBR's
} Nodo;
