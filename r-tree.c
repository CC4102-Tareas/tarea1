/**
	Estructura que permite persistir un R-tree. 
*/
#define NAME_FILE "r-tree.estructura"

/**
	Inicializa un r-tree.
	Crea un archivo de nombre 'r-tree.estructura'
*/
void init_rtree() {
	FILE *fptr;
	int status;
 
	status = remove(NAME_FILE);
 
	if(status == 0)
		printf("%s archivo eliminado.\n", NAME_FILE);
	else {	
		printf("Incapaz de eliminar archivo.\n");
		perror("Error");
	}

	// crea el archivo
	fptr = fopen(NAME_FILE, "w");
	fclose(fptr);
}

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
	fwrite(&(nodo.nodo_pader), sizeof(int), 1, fptr);
	fwrite(&(nodo_pos_mbr_padre), sizeof(int), 1, fptr);
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
        pftr = fopen(NAME_FILE, "w");

        if(fseek(pftr, nodo.nodo_id*TAMANO_PAGINA, SEEK_SET)) {
                printf("Error al intentar posicionarse en la página.");
        }

        fwrite(fptr, sizeof(int), 1, nodo.nodo_id);
	fwrite(fptr, sizeof(int), 1, nodo.nodo_padre);
        fwrite(fptr, sizeof(int), 1, nodo.pos_mbr_adre);
        fwrite(fptr, sizeof(int), 1, nodo.ultimo);
        fwrite(fptr, sizeof(MBR), 2*T, nodo.mbr);

        fclose(pftr);
}
/**
	elimina un nodo
*/







