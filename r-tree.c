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
	fptr = fopen(NAME_FILE, "r+");

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
void insertar_nodo_en_disco(Nodo nodo) {
	FILE *fptr;
    int ultimo_nodo_id;

    // se abre el archivo para lectura
    fptr = fopen(NAME_FILE, "r+");
    
    if (nodo.nodo_id == -1) {
        // se posiciona en el último nodo insertado
        if(fseek(fptr, -1*TAMANO_PAGINA, SEEK_END)) {
            printf("Error al intentar posicionarse en el último nodo insertado.\n");
        }
        // se lee el id
	    fread(&(ultimo_nodo_id), sizeof(int), 1, fptr);
        nodo.nodo_id = ultimo_nodo_id+1;
        printf("nodo_id= %d\n", ultimo_nodo_id+1);
    }

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
void actualizar_nodo_en_disco(Nodo nodo) {
    FILE *fptr;

    // se abre el archivo para lectura
    fptr = fopen(NAME_FILE, "r+");

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
		printf("Archivo %s eliminado.\n", NAME_FILE);
	else {	
		//printf("Incapaz de eliminar archivo.\n");
		//perror("Error");
	}

	// crea el archivo
	fptr = fopen(NAME_FILE, "w");
    printf("Archivo %s creado exitosamente.\n", NAME_FILE);

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
	
    insertar_nodo_en_disco(nodo);

    return nodo;
}

