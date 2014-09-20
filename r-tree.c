/**
	Estructura que permite persistir un R-tree. 

    Nodo leer_nodo_en_disco(int nodo_id)
    void insertar_nodo_en_disco(Nodo nodo)
    void actualizar_nodo_en_disco(Nodo nodo) {
    Nodo init_rtree(Rectangulo rect1, Rectangulo rect2)

*/
#define NAME_FILE "r-tree.estructura"

/**
	Leer nodo de un r-tree
*/
Nodo leer_nodo_en_disco(int numero_nodo) {
	FILE *fptr;
	
	// se abre el archivo para lectura
	fptr = fopen(NAME_FILE, "rb");

	if(fseek(fptr, numero_nodo*TAMANO_PAGINA, SEEK_SET)) {
		printf("Error al intentar posicionarse en la página.");	
	}

	Nodo nodo;

	//crear nodo con los datos leidos
	fread(&(nodo), sizeof(Nodo), 1, fptr);

	fclose(fptr);

	return nodo;
}

/**
	inserta un nodo en el R-tree
*/
void insertar_nodo_en_disco(Nodo nodo) {
	FILE *fptr;
    int ultimo_nodo_id;

    // se abre el archivo para lectura
    fptr = fopen(NAME_FILE, "a+b");
    
	// se posiciona al final del archivo
    //if(fseek(fptr, 0, SEEK_END)) {
    //    printf("Error al intentar posicionarse en la página.");
    //}

	fwrite(&(nodo), sizeof(Nodo), 1, fptr);

    fclose(fptr);
}

/**
	actualiza un nodo
*/
void actualizar_nodo_en_disco(Nodo nodo) {
    FILE *fptr;

    // se abre el archivo para lectura
    fptr = fopen(NAME_FILE, "r+b");

    if(fseek(fptr, nodo.nodo_id*TAMANO_PAGINA, SEEK_SET)) {
        printf("Error al intentar posicionarse en la página.");
    }

	fwrite(&(nodo), sizeof(Nodo), 1, fptr);

    fclose(fptr);
}
/**
	elimina un nodo
*/


/**
	Inicializa un r-tree.
	Crea un archivo de nombre 'r-tree.estructura' y retorna el
	nodo raiz del r.tree
*/
Nodo init_rtree() {
	FILE *fptr;
 
	// crea el archivo
	fptr = fopen(NAME_FILE, "wb");
    printf("Archivo %s creado exitosamente.\n", NAME_FILE);
	fclose(fptr);
	
	Nodo nodo;
	nodo.nodo_id = 0;
	nodo.nodo_padre = -1; // indica que es la raiz
	nodo.pos_mbr_padre = 0;
	nodo.ultimo = -1; // está vacío
    
    insertar_nodo_en_disco(nodo);

    return nodo;
}

