/**
	Estructura que permite persistir un R-tree. 
*/
#define NAME_FILE "r-tree.estructura"

/**
	Leer nodo de un r-tree
*/
Nodo leer_nodo(int numero_nodo) {
	FILE *fptr;
	
	// se abre el archivo para lectura
	fptr = fopen(NAME_FILE, "r");

	if(fseek(fptr, numero_nodo*TAMANO_PAGINA, SEEK_SET)) {
		printf("Error al intentar posicionarse en la página.");	
	}

	Nodo nodo;

	//crear nodo con los datos leidos
	fread(&(nodo.nodo_id), sizeof(int), 1, fptr);
	fread(&(nodo.nodo_padre), sizeof(int), 1, fptr);
        fread(&(nodo.pos_mbr_padre), sizeof(int), 1, fptr);
        fread(&(nodo.ultimo), sizeof(int), 1, fptr);
        fread(&(nodo.nodo_padre), sizeof(MBR), 2*T, fptr);

	fclose(fptr);

	return nodo;
}

/**
	inserta un nodo en el R-tree
*/
void insertar_nodo(Nodo nodo) {
	FILE *fptr;

        // se abre el archivo para lectura
        fptr = fopen(NAME_FILE, "w");

	// se posiciona al final del archivo
        if(fseek(fptr, 0, SEEK_END)) {
                printf("Error al intentar posicionarse en la página.");
        }

	fwrite(&(nodo.nodo_id), sizeof(int), 1, fptr);
	fwrite(&(nodo.nodo_padre), sizeof(int), 1, fptr);
	fwrite(&(nodo.pos_mbr_padre), sizeof(int), 1, fptr);
	fwrite(&(nodo.ultimo), sizeof(int), 1, fptr);
        fwrite(&(nodo.mbr), sizeof(MBR), 2*T, fptr);

        fclose(fptr);
}

/**
	actualiza un nodo
*/
actualizar_nodo(Nodo nodo) {
        FILE *fptr;

        // se abre el archivo para lectura
        fptr = fopen(NAME_FILE, "w");

        if(fseek(fptr, nodo.nodo_id*TAMANO_PAGINA, SEEK_SET)) {
                printf("Error al intentar posicionarse en la página.");
        }

	fwrite(&(nodo.nodo_id), sizeof(int), 1, fptr);
        fwrite(&(nodo.nodo_padre), sizeof(int), 1, fptr);
        fwrite(&(nodo.pos_mbr_padre), sizeof(int), 1, fptr);
        fwrite(&(nodo.ultimo), sizeof(int), 1, fptr);
        fwrite(&(nodo.mbr), sizeof(MBR), 2*T, fptr);

        fclose(fptr);
}
/**
	elimina un nodo
*/


/**
	Inicializa un r-tree.
	Crea un archivo de nombre 'r-tree.estructura' y retorna el
	nodo raiz del r.tree
    Recibe dos rectangulos porque es la cantidad mínima que puede 
    recibir la raiz.
*/
Nodo init_rtree(Rectangulo rect1, Rectangulo rect2) {
	FILE *fptr;
	int status;
 
	status = remove(NAME_FILE);
 
	if(status == 0)
		printf("%s archivo eliminado.\n", NAME_FILE);
	else {	
		//printf("Incapaz de eliminar archivo.\n");
		//perror("Error");
	}

	// crea el archivo
	fptr = fopen(NAME_FILE, "w");

	fclose(fptr);
	
	Nodo nodo;
	nodo.nodo_id = 0;
	nodo.nodo_padre = 0;
	nodo.pos_mbr_padre = 0;
	nodo.ultimo = 1;
    
    // la raíz tiene como mínimo 2 rectangulos.   
    // nuestro código modela una hoja como un MBR con nodo_hijo=-1
    MBR mbr1;
    MBR mbr2;
    
    mbr1.rect = rect1;
    mbr1.nodo_hijo = -1;
    mbr2.rect = rect2;
    mbr2.nodo_hijo = -1;
    
    // se agregan al nodo raíz
    nodo.mbr[0] = mbr1;
    nodo.mbr[1] = mbr2;
	
	insertar_nodo(nodo);
}

